# ******************************************************************************
# nodejs.pro                                                       Tao3D project
# ******************************************************************************
#
# File description:
# Qt build file for the NodeJS module
#
#
#
#
#
#
#
# ******************************************************************************
# This software is licensed under the GNU General Public License v3
# (C) 2019, Christophe de Dinechin <christophe@dinechin.org>
# (C) 2013, Jérôme Forissier <jerome@taodyne.com>
# ******************************************************************************
# This file is part of Tao3D
#
# Tao3D is free software: you can r redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Tao3D is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Tao3D, in a file named COPYING.
# If not, see <https://www.gnu.org/licenses/>.
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

