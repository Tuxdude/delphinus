#
#   toolchain.mk - toolchain binary definitions used by the 
#   makesystem for delphinus
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

ifndef DELPHINUS_TOOLCHAIN_MK
DELPHINUS_TOOLCHAIN_MK := 1

include $(BASE_DIR)/tools/utils.mk

# Host tool chain
HOST_TOOLCHAIN_PREFIX      ?=
# mingw-w32 tool chain
MINGW_W32_TOOLCHAIN_PREFIX ?= i686-w64-mingw32-
# mingw-w64 tool chain
MINGW_W64_TOOLCHAIN_PREFIX ?= x86_64-w64-mingw32-

# Determine which toolchains are present, only when config.mk doesn't
# know about it - which is the case as of now.
# If present, generate the architecture names from the compiler info
ifeq ($(CONFIG_SELECTED_TOOLCHAINS),no)
    FIND_TOOLCHAIN_CMD_OPTIONS := 1>/dev/null 2>/dev/null && echo 1
    ifeq ($(shell which $(HOST_TOOLCHAIN_PREFIX)gcc $(FIND_TOOLCHAIN_CMD_OPTIONS)),1)
        ARCH_HOST := $(shell $(HOST_TOOLCHAIN_PREFIX)gcc -dumpmachine)
        ALL_ARCHS += $(ARCH_HOST)
    endif
    ifeq ($(shell which $(MINGW_W32_TOOLCHAIN_PREFIX)gcc $(FIND_TOOLCHAIN_CMD_OPTIONS)),1)
        ARCH_MINGW_W32 := $(shell $(MINGW_W32_TOOLCHAIN_PREFIX)gcc -dumpmachine)
        ALL_ARCHS += $(ARCH_MINGW_W32)
    endif
    ifeq ($(shell which $(MINGW_W64_TOOLCHAIN_PREFIX)gcc $(FIND_TOOLCHAIN_CMD_OPTIONS)),1)
        ARCH_MINGW_W64 := $(shell $(MINGW_W64_TOOLCHAIN_PREFIX)gcc -dumpmachine)
        ALL_ARCHS += $(ARCH_MINGW_W64)
    endif
else
ifneq ($(CONFIG_SELECTED_TOOLCHAINS),yes)
    $(error Invalid config for CONFIG_SELECTED_TOOLCHAINS, aborting...)
else
# The configure script has already found out the toolchains to be used
# Generate architecture names only for those and add them to ALL_ARCHS
# FIXME
endif
endif

ifneq ($(ARCH),)
    TOOLCHAIN_ARCH_CFLAGS   :=
    TOOLCHAIN_ARCH_CXXFLAGS :=
    TOOLCHAIN_ARCH_LDFLAGS  :=
    ifneq ($(filter-out $(ALL_ARCHS),$(ARCH)),)
        $(error Invalid value for ARCH, aborting...)
    endif
    # Now choose the toolchain and architecture specific flags based on ARCH
    ifeq ($(ARCH),$(ARCH_HOST))
        TOOLCHAIN_PREFIX        := $(HOST_TOOLCHAIN_PREFIX)
        TOOLCHAIN_ARCH_CFLAGS   += -fPIC
        TOOLCHAIN_ARCH_CXXFLAGS += -fPIC
        TOOLCHAIN_ARCH_LDFLAGS  += -rdynamic
    endif
    ifeq ($(ARCH),$(ARCH_MINGW_W32))
        TOOLCHAIN_PREFIX        := $(MINGW_W32_TOOLCHAIN_PREFIX)
        TOOLCHAIN_ARCH_CFLAGS   += -Wno-pedantic-ms-format
        TOOLCHAIN_ARCH_CXXFLAGS += -Wno-pedantic-ms-format
    endif
    ifeq ($(ARCH),$(ARCH_MINGW_W64))
        TOOLCHAIN_PREFIX     := $(MINGW_W64_TOOLCHAIN_PREFIX)
        TOOLCHAIN_ARCH_CFLAGS   += -Wno-pedantic-ms-format
        TOOLCHAIN_ARCH_CXXFLAGS += -Wno-pedantic-ms-format
    endif

    CC      := $(TOOLCHAIN_PREFIX)gcc
    CXX     := $(TOOLCHAIN_PREFIX)g++
    AS      := $(TOOLCHAIN_PREFIX)as
    AR      := $(TOOLCHAIN_PREFIX)ar
    LD      := $(TOOLCHAIN_PREFIX)ld
    RANLIB  := $(TOOLCHAIN_PREFIX)ranlib
    SIZE    := $(TOOLCHAIN_PREFIX)size
    STRIP   := $(TOOLCHAIN_PREFIX)strip
    OBJCOPY := $(TOOLCHAIN_PREFIX)objcopy
    NM      := $(TOOLCHAIN_PREFIX)nm
endif

endif
