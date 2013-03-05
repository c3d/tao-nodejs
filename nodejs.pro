# ******************************************************************************
#  nodejs.pro                                                       Tao project
# ******************************************************************************
# File Description:
# Qt build file for the NodeJS module
# ******************************************************************************
# This software is property of Taodyne SAS - Confidential
# Ce logiciel est la propriété de Taodyne SAS - Confidentiel
# (C) 2013 Jerome Forissier <jerome@taodyne.com>
# (C) 2013 Taodyne SAS
# ******************************************************************************

MODINSTDIR = nodejs

include(../modules.pri)

HEADERS     = nodejs.h
SOURCES     = nodejs.cpp
TBL_SOURCES = nodejs.tbl
OTHER_FILES = nodejs.xl nodejs.tbl traces.tbl
QT         += core

# Icon is a capture of a part of the NodeJS logo (http://nodejs.org)
INSTALLS += thismod_icon

QMAKE_SUBSTITUTES = doc/Doxyfile.in
DOXYFILE = doc/Doxyfile
DOXYLANG = en,fr
include(../modules_doc.pri)

