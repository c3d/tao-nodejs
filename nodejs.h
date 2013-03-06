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
    static XL::Name_p      nodejs(text name, text src);
    static XL::Name_p      nodejs(text src) { return nodejs("default", src); }

public:
    bool                   init();
    void                   stopAll();

    NodeJSProcess *        run(text name, text src);

protected:
    std::ostream &         debug();

protected:
    typedef QMap<QString, NodeJSProcess *>  proc_map;

protected:
    proc_map               processes;
    const Tao::ModuleApi * tao;

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
    virtual ~NodeJSProcess();

protected:
    std::ostream &         debug();

protected slots:
    void                   onReadyReadStandardOutput();
    void                   onReadyReadStandardError();

public:
    QString                name, src;

protected:
    QByteArray             out;
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

