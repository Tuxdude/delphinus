#
#   config.mk - configure the makesystem and the options for delphinus
#
#   This file is part of delphinus - a stream analyzer for various multimedia
#   stream formats and containers.
#
#   Copyright (C) 2011 Ashwin (Tuxdude)
#   tuxdude@users.sourceforge.net
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

ifndef DELPHINUS_CONFIG_MK
DELPHINUS_CONFIG_MK := 1

include $(BASE_DIR)/tools/toolchain.mk

DELPHINUS := delphinus

# Leaving this variable blank builds all architectures
# Add architecture names from toolchain.mk into this variable to build
# only for those architectures
# Set only to build for host machine as of now
# mingw-w32 and mingw-w64 also works
ONLY_BUILD_ARCHS := $(ARCH_HOST)

# Directory names where source/built files will be placed or exported
EXPORT_BASE_DIR_NAME := dist
EXPORT_RELEASES_DIR_NAME := release
EXPORT_HEADERS_DIR_NAME := include
EXPORT_LIBS_DIR_NAME := lib
EXPORT_BINS_DIR_NAME := bin
DEP_DIR_NAME := .dep

# If you need to append to any of these flags universally, this is the 
# right place to do it
# Flags can also be set based on ARCH
# NOTE: Do not remove the empty initialization as immediate variables,
# failing which Makefile performance will degrade
ARCH_FLAGS :=
OPTIMIZATION_FLAGS :=
WARN_FLAGS :=
WARN_C_FLAGS :=
WARN_CXX_FLAGS :=
COMMON_FLAGS :=
INC_PATH :=
CFLAGS :=
CXXFLAGS :=
CPPFLAGS :=
LDFLAGS :=

# Final flag to indicate config.mk was actually parsed
DELPHINUS_CONFIGURED := yes

endif
