#
#   Makefile - Makefile for tsinfo
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

BASE_DIR := ..

include $(BASE_DIR)/tools/config.mk
BUILD_ARCHS := $(ALL_ARCHS)

sources := tsinfo.cpp
objs := $(addprefix $(ARCH)/,$(sources:.cpp=.o))

SOURCES := $(sources)
TARGET = $(ARCH)/tsinfo
EXPORT_BINS = $(TARGET)
PRE_REQS := libdelphinus

ifneq ($(ARCH),$(ARCH_HOST))
    TARGET = $(ARCH)/tsinfo.exe
endif

include $(BASE_DIR)/tools/makesystem.mk

CPPFLAGS += -D_FILE_OFFSET_BITS=64
LDFLAGS += -ldelphinus

$(TARGET): $(objs)
	$(LINK)
