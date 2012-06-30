#
#   config.mk - configure the makesystem and the options for delphinus
#
#   This file is part of delphinus.
#
#   Copyright (C) 2012 Ash (Tuxdude) <tuxdude.github@gmail.com>
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU Lesser General Public License as
#   published by the Free Software Foundation; either version 3 of the
#   License, or (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU Lesser General Public
#   License along with this program.  If not, see
#   <http://www.gnu.org/licenses/>.
#

ifndef DELPHINUS_CONFIG_MK
DELPHINUS_CONFIG_MK := 1

DELPHINUS := delphinus
CONFIG_SELECTED_TOOLCHAINS := no

include $(BASE_DIR)/tools/toolchain.mk

# Leaving this variable blank builds all architectures
# Add architecture names from toolchain.mk into this variable to build
# only for those architectures
# Set only to build for host machine as of now
# mingw-w32 and mingw-w64 also works
ONLY_BUILD_ARCHS ?=

# Enable a verbose output from makesystem
VERBOSE ?= no

# Enable the bizzare pedantic warnings
PEDANTIC ?= no

# Disable the colorized output from make
DISABLE_COLORS ?= no

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

# Architecture specific flags (set it based on ARCH)
ARCH_FLAGS         ?=
# Optimization flags
OPTIMIZATION_FLAGS ?=
# Common Warning flags (used for both C and C++)
WARN_FLAGS         ?=
# Warning flags (used only for C programs)
WARN_C_FLAGS       ?=
# Warning flags (used only for C++ programs)
WARN_CXX_FLAGS     ?=
# Any other common flags (used for both C and C++)
COMMON_FLAGS       ?=
# Include 
INC_FLAGS          ?=
# The usual CFLAGS
CFLAGS             ?=
# The usual CXXFLAGS
CXXFLAGS           ?=
# The usual CPPFLAGS
CPPFLAGS           ?=
# The usual LDFLAGS
LDFLAGS            ?=


# Final flag to indicate config.mk was actually parsed
DELPHINUS_CONFIGURED := yes

endif
