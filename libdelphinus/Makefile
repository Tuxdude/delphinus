#
#   Makefile - Makefile for delphinus
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


sources := Ts.cpp Pes.cpp PsiTables.cpp TsFile.cpp
BASE_DIR := ..

include $(BASE_DIR)/tools/toolchain.mk
BUILD_ARCHS := $(ARCH_HOST) $(ARCH_MINGW_W32) $(ARCH_MINGW_W64)

objs = $(addprefix $(ARCH)/,$(sources:.cpp=.o))

SOURCES := $(sources)
TARGET = $(ARCH)/libdelphinus.so
EXPORT_HEADERS_PREFIX_DIR := libdelphinus
EXPORT_HEADERS := Ts.h TsFile.h Pes.h
EXPORT_LIBS = $(TARGET)

ifneq ($(ARCH),$(ARCH_HOST))
    TARGET = $(ARCH)/libdelphinus.dll
endif

include $(BASE_DIR)/tools/makesystem.mk

CPPFLAGS += -D_FILE_OFFSET_BITS=64

$(TARGET): $(objs)
	$(LINK_SHARED)
