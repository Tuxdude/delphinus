#
#   utils.mk - set of utility definitions used by the makesystem for delphinus
#
#   This file is part of delphinus - a stream analyzer for various multimedia
#   stream formats and containers.
#
#   Copyright (C) 2012 Ash (Tuxdude)
#   tuxdude.github@gmail.com
#
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

ifndef DELPHINUS_UTILS_MK
DELPHINUS_UTILS_MK := 1

TEST         := test
ECHO         := echo -e
CAT          := cat
TOUCH        := touch
CD           := cd
CP           := cp
MV           := mv
RM           := rm -f
RM_RECURSIVE := rm -rf
MKDIR        := mkdir -p
RMDIR        := rmdir --ignore-fail-on-non-empty
GREP         := grep
SED          := sed
SVN          := svn
TAR          := tar
DOXYGEN      := doxygen
CLOC         := cloc

endif
