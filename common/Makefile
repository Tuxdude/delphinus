#
#   Makefile - Makefile for common
#
#   This file is part of delphinus - a stream analyzer for various multimedia
#   stream formats and containers.
#
#   Copyright (C) 2011 Ash (Tuxdude)
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

sources := DelphinusUtils.cpp
BASE_DIR := ..

include $(BASE_DIR)/tools/config.mk
BUILD_ARCHS := $(ALL_ARCHS)

objs = $(addprefix $(ARCH)/,$(sources:.cpp=.o))

SOURCES := $(sources)
TARGET = $(ARCH)/libdelphinuscommon.a
EXPORT_HEADERS_PREFIX_DIR := common
EXPORT_HEADERS := DelphinusUtils.h
EXPORT_LIBS = $(TARGET)

ifneq ($(ARCH),$(ARCH_HOST))
    TARGET = $(ARCH)/libdelphinuscommon.dll
endif

include $(BASE_DIR)/tools/makesystem.mk

CXXFLAGS += -DDELPHINUS_IN_COMMON_DIR

$(TARGET): $(objs)
	$(LINK_STATIC)
