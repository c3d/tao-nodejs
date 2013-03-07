// ****************************************************************************
//  nodejs.cpp                                                     Tao project
// ****************************************************************************
//
//   File Description:
//
//    Run JavaScript code in a NodeJS subprocess. Allow Tao to JavaScript
//    procedure calls and vice-versa.
//
//
//
//
//
//
//
// ****************************************************************************
// This software is property of Taodyne SAS - Confidential
// Ce logiciel est la propriété de Taodyne SAS - Confidentiel
//  (C) 2013 Jerome Forissier <jerome@taodyne.com>
//  (C) 2013 Taodyne SAS
// ****************************************************************************

#include "nodejs.h"
#include "runtime.h"
#include "context.h"
#include <QRegExp>
#include <QEvent>


NodeJSFactory * NodeJSFactory::inst = NULL;


NodeJSFactory::NodeJSFactory(const Tao::ModuleApi *tao)
// ----------------------------------------------------------------------------
//   Factory constructor
// ----------------------------------------------------------------------------
    : tao(tao)
{}


NodeJSFactory::~NodeJSFactory()
// ----------------------------------------------------------------------------
//   Factory destructor
// ----------------------------------------------------------------------------
{}


XL::Name_p NodeJSFactory::nodejs(XL::Context *context, XL::Tree_p self,
                                 text name, text src)
// ----------------------------------------------------------------------------
//   Start a NodeJS subprocess to execute JavaScript code
// ----------------------------------------------------------------------------
{
    NodeJSFactory * f = instance();
    f->tao->refreshOn(QEvent::Timer, f->tao->currentTime() + 0.1 /*-1.0*/);
    if (f->processes.contains(+name))
    {
        NodeJSProcess * proc = f->processes[+name];
        if (+src != proc->src)
        {
            IFTRACE(nodejs)
                f->debug() << "Source code changed\n";
            delete proc;
            f->run(name, src);
        }
        else
        {
            // See if we have pending callbacks to run in the XL context
            proc->runCallbacks(context, self);
            Q_UNUSED(context);
        }
        return XL::xl_true;
    }

    f->run(name, src);
    return XL::xl_true;
}


XL::Name_p NodeJSFactory::nodejs_writeln(text name, text msg)
// ----------------------------------------------------------------------------
//   Send text to the standard input of a subprocess
// ----------------------------------------------------------------------------
{
    NodeJSFactory * f = instance();
    if (!f->processes.contains(+name))
        return XL::xl_false;

    NodeJSProcess * proc = f->processes[+name];
    proc->writeToStdin(msg);
    return XL::xl_true;
}


NodeJSFactory * NodeJSFactory::instance(const Tao::ModuleApi *tao)
// ----------------------------------------------------------------------------
//   Create/return instance of the factory
// ----------------------------------------------------------------------------
{
    if (!inst)
        inst = new NodeJSFactory(tao);
    return inst;
}


std::ostream & NodeJSFactory::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[NodeJSFactory] ";
    return std::cerr;
}


bool NodeJSFactory::init()
// ----------------------------------------------------------------------------
//   Initialize the factory, return true on success
// ----------------------------------------------------------------------------
{
    QString prog = "node";
    QStringList args;
    args << "-v";
    QProcess node;
    node.start(prog, args);
    if (node.waitForFinished())
    {
        IFTRACE(nodejs)
            debug() << "Module initialized\n";
        return true;
    }
    IFTRACE(nodejs)
        debug() << "Failed to execute 'node' command\n";
    return false;
}


void NodeJSFactory::stopAll()
// ----------------------------------------------------------------------------
//   Stop all subprocesses
// ----------------------------------------------------------------------------
{
    foreach (NodeJSProcess * proc, processes)
        delete proc;
}


NodeJSProcess * NodeJSFactory::run(text name, text src)
// ----------------------------------------------------------------------------
//   Create new NodeJS subprocess named 'name' to run 'src'
// ----------------------------------------------------------------------------
{
    NodeJSProcess * proc = new NodeJSProcess(this, +name, +src);
    processes[+name] = proc;
    return proc;
}


// ============================================================================
//
//    NodeJS subprocess
//
// ============================================================================

NodeJSProcess::NodeJSProcess(QObject *parent, const QString name,
                             const QString src)
// ----------------------------------------------------------------------------
//   Create and start a subprocess and make it run the specified code
// ----------------------------------------------------------------------------
    : QProcess(parent), name(name), src(src)
{
    std::string path = NodeJSFactory::instance()->tao->currentDocumentFolder();
    setWorkingDirectory(+path);

    IFTRACE(nodejs)
        debug() << "Starting node process (wd: " << path << ")\n";

    connect(this, SIGNAL(readyReadStandardOutput()),
            this, SLOT(onReadyReadStandardOutput()));
    connect(this, SIGNAL(readyReadStandardError()),
            this, SLOT(onReadyReadStandardError()));

    QString node = "node";
    QStringList args;
    args << "-e" << src;
    start(node, args);
}


NodeJSProcess::~NodeJSProcess()
// ----------------------------------------------------------------------------
//   Stop/kill subprocess
// ----------------------------------------------------------------------------
{
    IFTRACE(nodejs)
        debug() << "Destructor\n";
    close();
}


std::ostream & NodeJSProcess::debug()
// ----------------------------------------------------------------------------
//   Convenience method to log with a common prefix
// ----------------------------------------------------------------------------
{
    std::cerr << "[NodeJS '" << +name << "'] ";
    return std::cerr;
}


void NodeJSProcess::onReadyReadStandardOutput()
// ----------------------------------------------------------------------------
//   Read standard output of subprocess, filter out callback requests
// ----------------------------------------------------------------------------
{
    QByteArray ba = readAllStandardOutput();
    out.append(ba);
    int eol = out.indexOf('\n');
    while (eol >= 0)
    {
        QByteArray line = out.left(eol);
        QString sline = QString::fromLocal8Bit(line);
        IFTRACE(nodejs)
            debug() << "stdout [" << +sline << "]\n";

        static QRegExp re("^CB/(.*)");
        if (re.indexIn(sline) != -1)
        {
            // Callback command for Tao
            IFTRACE(nodejs)
                debug() << "Pushing callback request\n";
            commands.append(re.cap(1));
        }
        else
        {
            // Any other output is forwarded to Tao's stdout
            std::cout << +sline << "\n";
        }

        int keep = out.length() - eol - 1;
        Q_ASSERT(keep >= 0);
        out = out.right(keep);
        eol = out.indexOf('\n');
    }
}


void NodeJSProcess::onReadyReadStandardError()
// ----------------------------------------------------------------------------
//   Read standard error of subprocess
// ----------------------------------------------------------------------------
{
    std::cerr << +QString::fromLocal8Bit(readAllStandardError());
}


void NodeJSProcess::runCallbacks(XL::Context *context, XL::Tree_p self)
// ----------------------------------------------------------------------------
//   Execute pending callbacks in given context
// ----------------------------------------------------------------------------
{
    Q_UNUSED(context);
    while (!commands.isEmpty())
    {
        std::string code = +commands.takeFirst();
        IFTRACE(nodejs)
            debug() << "Execute callback: " << code << "\n";

        XL::Tree *tree = XL::xl_parse_text(code);
        if (!tree)
            return;
        if (XL::Symbols *syms = self->Symbols())
            tree->SetSymbols(syms);
        context->Evaluate(tree);
    }
}


void NodeJSProcess::writeToStdin(text msg)
// ----------------------------------------------------------------------------
//   Send text to the standard input of the process
// ----------------------------------------------------------------------------
{
    IFTRACE(nodejs)
        debug() << "stdin [" << msg << "]\n";
    QByteArray ba = QByteArray::fromRawData(msg.c_str(), msg.size());
    write(ba);
    write("\n");
}



// ============================================================================
//
//    Globals
//
// ============================================================================

XL_DEFINE_TRACES

int module_init(const Tao::ModuleApi *api, const Tao::ModuleInfo *mod)
// ----------------------------------------------------------------------------
//   Initialize the Tao module
// ----------------------------------------------------------------------------
{
    Q_UNUSED(mod);
    XL_INIT_TRACES();
    NodeJSFactory * factory = NodeJSFactory::instance(api);
    if (!factory->init())
        return -1;

    return 0;
}


int module_exit()
// ----------------------------------------------------------------------------
//   Uninitialize the Tao module
// ----------------------------------------------------------------------------
{
    NodeJSFactory::instance()->stopAll();
    return 0;
}

