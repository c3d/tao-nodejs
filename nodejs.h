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
#include <QString>
#include <iostream>


class NodeJSProcess;

class NodeJSFactory
// ----------------------------------------------------------------------------
//    Create, store and manage NodeJS subprocesses
// ----------------------------------------------------------------------------
{
public:
    NodeJSFactory(const Tao::ModuleApi *tao = 0);
    virtual ~NodeJSFactory() {}

public:
    static NodeJSFactory * instance(const Tao::ModuleApi *tao = 0);
    static void            destroy();

    // XL interface
    static XL::Name_p      nodejs_exec(text name, text file);

public:
    void                   stopAll();

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


#endif // NODEJS_H

