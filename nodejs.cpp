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


NodeJSFactory * NodeJSFactory::inst = NULL;


NodeJSFactory::NodeJSFactory(const Tao::ModuleApi *tao)
// ----------------------------------------------------------------------------
//   Factory constructor
// ----------------------------------------------------------------------------
    : tao(tao)
{}


XL::Name_p NodeJSFactory::nodejs_exec(text name, text file)
// ----------------------------------------------------------------------------
//   Start a NodeJS subprocess
// ----------------------------------------------------------------------------
{
    Q_UNUSED(file);

    NodeJSFactory * factory = instance();
    if (factory->processes.contains(+name))
        return XL::xl_true;

    return XL::xl_false;
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
}


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

