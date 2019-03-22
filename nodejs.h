#ifndef NODEJS_H
#define NODEJS_H
// *****************************************************************************
// nodejs.h                                                        Tao3D project
// *****************************************************************************
//
// File description:
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
// *****************************************************************************
// This software is licensed under the GNU General Public License v3
// (C) 2014,2019, Christophe de Dinechin <christophe@dinechin.org>
// (C) 2013, Jérôme Forissier <jerome@taodyne.com>
// *****************************************************************************
// This file is part of Tao3D
//
// Tao3D is free software: you can r redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Tao3D is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tao3D, in a file named COPYING.
// If not, see <https://www.gnu.org/licenses/>.
// *****************************************************************************

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
    void                   fileChanged();

protected:
    static void            fileChangedStatic(std::string path,
                                             std::string absolutePath,
                                             void *obj);

protected slots:
    void                   onReadyReadStandardOutput();
    void                   onReadyReadStandardError();

public:
    QString                name, src, file, error;
    int                    userEvent;

protected:
    QByteArray             out;
    QStringList            commands;
    void *                 fileMonitor;
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

