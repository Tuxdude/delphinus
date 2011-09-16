#
#   toolchain.mk - toolchain binary definitions used by the 
#   makesystem for delphinus
#
#   This file is part of delphinus - a stream analyzer for various multimedia
#   stream formats and containers.
#
#   Copyright (C) 2011 Ashwin Gururaghavendran
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
HOST_TOOLCHAIN_PREFIX :=
HOST_CC           := $(HOST_TOOLCHAIN_PREFIX)gcc
HOST_CXX          := $(HOST_TOOLCHAIN_PREFIX)g++
HOST_AS           := $(HOST_TOOLCHAIN_PREFIX)as
HOST_AR           := $(HOST_TOOLCHAIN_PREFIX)ar
HOST_LD           := $(HOST_TOOLCHAIN_PREFIX)ld
HOST_RANLIB       := $(HOST_TOOLCHAIN_PREFIX)ranlib
HOST_SIZE         := $(HOST_TOOLCHAIN_PREFIX)size
HOST_STRIP        := $(HOST_TOOLCHAIN_PREFIX)strip
HOST_OBJCOPY      := $(HOST_TOOLCHAIN_PREFIX)objcopy
HOST_NM           := $(HOST_TOOLCHAIN_PREFIX)nm

# mingw-w32 tool chain
MINGW_W32_TOOLCHAIN_PREFIX := i686-w64-mingw32-
MINGW_W32_CC      := $(MINGW_W32_TOOLCHAIN_PREFIX)gcc
MINGW_W32_CXX     := $(MINGW_W32_TOOLCHAIN_PREFIX)g++
MINGW_W32_AS      := $(MINGW_W32_TOOLCHAIN_PREFIX)as
MINGW_W32_AR      := $(MINGW_W32_TOOLCHAIN_PREFIX)ar
MINGW_W32_LD      := $(MINGW_W32_TOOLCHAIN_PREFIX)ld
MINGW_W32_RANLIB  := $(MINGW_W32_TOOLCHAIN_PREFIX)ranlib
MINGW_W32_SIZE    := $(MINGW_W32_TOOLCHAIN_PREFIX)size
MINGW_W32_STRIP   := $(MINGW_W32_TOOLCHAIN_PREFIX)strip
MINGW_W32_OBJCOPY := $(MINGW_W32_TOOLCHAIN_PREFIX)objcopy
MINGW_W32_NM      := $(MINGW_W32_TOOLCHAIN_PREFIX)nm

# mingw-w64 tool chain
MINGW_W64_TOOLCHAIN_PREFIX := x86_64-w64-mingw32-
MINGW_W64_CC      := $(MINGW_W64_TOOLCHAIN_PREFIX)gcc
MINGW_W64_CXX     := $(MINGW_W64_TOOLCHAIN_PREFIX)g++
MINGW_W64_AS      := $(MINGW_W64_TOOLCHAIN_PREFIX)as
MINGW_W64_AR      := $(MINGW_W64_TOOLCHAIN_PREFIX)ar
MINGW_W64_LD      := $(MINGW_W64_TOOLCHAIN_PREFIX)ld
MINGW_W64_RANLIB  := $(MINGW_W64_TOOLCHAIN_PREFIX)ranlib
MINGW_W64_SIZE    := $(MINGW_W64_TOOLCHAIN_PREFIX)size
MINGW_W64_STRIP   := $(MINGW_W64_TOOLCHAIN_PREFIX)strip
MINGW_W64_OBJCOPY := $(MINGW_W64_TOOLCHAIN_PREFIX)objcopy
MINGW_W64_NM      := $(MINGW_W64_TOOLCHAIN_PREFIX)nm

# Generate the architecture names from the compiler info
SED_EXTRACT_CMD := $(SED) 's/^\(Target:\) \(.*$$\)/\2/'
ARCH_HOST := $(shell $(HOST_CC) -v 2>&1 | $(GREP) 'Target: ' | $(SED_EXTRACT_CMD))
ARCH_MINGW_W32 := $(shell $(MINGW_W32_CC) -v 2>&1 | $(GREP) 'Target: ' | $(SED_EXTRACT_CMD))
ARCH_MINGW_W64 := $(shell $(MINGW_W64_CC) -v 2>&1 | $(GREP) 'Target: ' | $(SED_EXTRACT_CMD))

endif
