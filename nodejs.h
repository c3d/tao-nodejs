#ifndef NODEJS_H
#define NODEJS_H
// ****************************************************************************
//  nodejs.h                                                       Tao project
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

#include "tree.h"
#include "tao/module_api.h"
#include <QMap>
#include <QProcess>
#include <QString>
#include <iostream>


// ============================================================================
//
//    Process factory
//
// ============================================================================

class NodeJSProcess;

class NodeJSFactory : public QObject
// ----------------------------------------------------------------------------
//    Create, store and manage NodeJS subprocesses
// ----------------------------------------------------------------------------
{
public:
    NodeJSFactory(const Tao::ModuleApi *tao = 0);
    virtual ~NodeJSFactory();

public:
    static NodeJSFactory * instance(const Tao::ModuleApi *tao = 0);
    static void            destroy();

    // XL interface
    static XL::Name_p      nodejs(XL::Context *context, XL::Tree_p self,
                                  text name, text src);
    static XL::Name_p      nodejs(XL::Context *context, XL::Tree_p self,
                                  text src)
    {
        return nodejs(context, self, "default", src);
    }
    static XL::Name_p      nodejs_load(XL::Context *context, XL::Tree_p self,
                                  text name, text file);
    static XL::Name_p      nodejs_load(XL::Context *context, XL::Tree_p self,
                                  text file)
    {
        return nodejs_load(context, self, "default", file);
    }

    static XL::Name_p      nodejs_writeln(text name, text msg);
    static XL::Name_p      nodejs_writeln(text msg)
    {
        return nodejs_writeln("default", msg);
    }

public:
    bool                   init();
    void                   stopAll();

    NodeJSProcess *        run(text name, text src);
    NodeJSProcess *        runFile(text name, text file);

protected:
    std::ostream &         debug();

protected:
    typedef QMap<QString, NodeJSProcess *>  proc_map;

public:
    const Tao::ModuleApi * tao;

protected:
    proc_map               processes;

protected:
    static NodeJSFactory * inst;
};


// ============================================================================
//
//    NodeJS subprocess
//
// ============================================================================

class NodeJSProcess : public QProcess
// ----------------------------------------------------------------------------
//    A NodeJS subprocess
// ----------------------------------------------------------------------------
{
    Q_OBJECT

public:
    NodeJSProcess(QObject *parent, const QString name, const QString src);
    NodeJSProcess(QObject *parent, const QString name, const QString file,
                  bool unused);
    virtual ~NodeJSProcess();

public:
    void                   runCallbacks(XL::Context *context, XL::Tree_p self);
    void                   writeToStdin(text msg);

protected:
    std::ostream &         debug();

protected slots:
    void                   onReadyReadStandardOutput();
    void                   onReadyReadStandardError();

public:
    QString                name, src, file;

protected:
    QByteArray             out;
    QStringList            commands;
};


// ============================================================================
//
//    Helpers
//
// ============================================================================

inline QString operator +(std::string s)
// ----------------------------------------------------------------------------
//   UTF-8 conversion from std::string to QString
// ----------------------------------------------------------------------------
{
    return QString::fromUtf8(s.data(), s.length());
}

inline std::string operator +(QString s)
// ----------------------------------------------------------------------------
//   UTF-8 conversion from QString to std::string
// ----------------------------------------------------------------------------
{
    return std::string(s.toUtf8().constData());
}


#endif // NODEJS_H

