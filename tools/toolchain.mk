#
#   toolchain.mk - toolchain binary definitions used by the 
#   makesystem for delphinus
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

ifndef DELPHINUS_TOOLCHAIN_MK
DELPHINUS_TOOLCHAIN_MK := 1

include $(BASE_DIR)/tools/utils.mk

FIND_TOOLCHAIN_CMD_OPTIONS := 1>/dev/null 2>/dev/null && echo 1
define SetupArchNames
    ifeq ($(shell which $($(1)_TOOLCHAIN_PREFIX)gcc $(FIND_TOOLCHAIN_CMD_OPTIONS)),1)
        ARCH_$(1) := $(shell $($(1)_TOOLCHAIN_PREFIX)gcc -dumpmachine)
        ALL_ARCHS += $$(ARCH_$(1))
    endif
endef

define SetupArchFlags
    ifeq ($(ARCH),$(ARCH_$(1)))
        TOOLCHAIN_PREFIX        := $$($(1)_TOOLCHAIN_PREFIX)
        TOOLCHAIN_ARCH_CFLAGS   += $$($(1)_CFLAGS)
        TOOLCHAIN_ARCH_CXXFLAGS += $$($(1)_CXXFLAGS)
        TOOLCHAIN_ARCH_LDFLAGS  += $$($(1)_LDFLAGS)
    endif
endef

TOOLCHAINS := HOST MINGW_W32 MINGW_W64

# Toolchain prefixes
# Host tool chain
HOST_TOOLCHAIN_PREFIX      ?=
# mingw-w32 tool chain
MINGW_W32_TOOLCHAIN_PREFIX ?= i686-w64-mingw32-
# mingw-w64 tool chain
MINGW_W64_TOOLCHAIN_PREFIX ?= x86_64-w64-mingw32-

# Arch specific flags
HOST_CFLAGS        := -fPIC
HOST_CXXFLAGS      := -fPIC
HOST_LDFLAGS       := -rdynamic
MINGW_W32_CFLAGS   := -Wno-pedantic-ms-format
MINGW_W32_CXXFLAGS := -Wno-pedantic-ms-format
MINGW_W32_LDFLAGS  :=
MINGW_W64_CFLAGS   := -Wno-pedantic-ms-format
MINGW_W64_CXXFLAGS := -Wno-pedantic-ms-format
MINGW_W64_LDFLAGS  :=

# Determine which toolchains are present, only when config.mk doesn't
# know about it - which is the case as of now.
ALL_ARCHS :=
ifeq ($(CONFIG_SELECTED_TOOLCHAINS),no)
    # Generate the architecture names from the compiler info for all
    # the configured archs.
    $(foreach toolchain,$(TOOLCHAINS),$(eval $(call SetupArchNames,$(toolchain))))
else
    ifneq ($(CONFIG_SELECTED_TOOLCHAINS),yes)
        $(error Invalid config for CONFIG_SELECTED_TOOLCHAINS, aborting...)
    else
        # The configure script has already found out the toolchains to be used
        # Generate architecture names only for those and add them to ALL_ARCHS
        # FIXME
        #-include tools/toolchains-info.mk
    endif
endif

# Now set the toolchain binaries only if ARCH is set
# we have no other way to determine otherwise
ifneq ($(ARCH),)
    # If an invalid ARCH is set, bail out
    ifneq ($(filter-out $(ALL_ARCHS),$(ARCH)),)
        $(error Invalid value for ARCH, aborting...)
    endif

    # Choose the toolchain and architecture specific flags based on ARCH
    $(foreach toolchain,$(TOOLCHAINS),$(eval $(call SetupArchFlags,$(toolchain))))

    # Set up the binaries using the toolchain prefix
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
