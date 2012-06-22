#
#   colors.mk - Hacks to print the output colorized
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

ifndef DELPHINUS_COLORS_MK
DELPHINUS_COLORS_MK := 1

include $(BASE_DIR)/tools/config.mk

ifneq ($(DISABLE_COLORS),yes)
    COLOR_BLACK   := \x1b[1;30m
    COLOR_RED     := \x1b[1;31m
    COLOR_GREEN   := \x1b[1;32m
    COLOR_YELLOW  := \x1b[1;33m
    COLOR_BLUE    := \x1b[1;34m
    COLOR_MAGENTA := \x1b[1;35m
    COLOR_CYAN    := \x1b[1;36m
    COLOR_WHITE   := \x1b[1;37m
    COLOR_RESET   := \x1b[0m
else
    COLOR_BLACK   := 
    COLOR_RED     :=
    COLOR_GREEN   :=
    COLOR_YELLOW  :=
    COLOR_BLUE    :=
    COLOR_MAGENTA :=
    COLOR_CYAN    :=
    COLOR_WHITE   :=
    COLOR_RESET   :=
endif

COLOR_WARNINGS   := $(COLOR_YELLOW)
COLOR_ERRORS     := $(COLOR_RED)
COLOR_COMPILE_OK := $(COLOR_GREEN)
COLOR_LINK_OK    := $(COLOR_BLUE)
COLOR_EXPORT     := $(COLOR_MAGENTA)
COLOR_CLEAN      := $(COLOR_CYAN)


endif
