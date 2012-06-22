/*
 *  DelphinusUtils.cpp - Common set of types and helper functions used by the
 *  rest of the delphinus project.
 *
 *  This file is part of delphinus - a stream analyzer for various multimedia
 *  stream formats and containers.
 *
 *  Copyright (C) 2011 Ash (Tuxdude)
 *  tuxdude.github@gmail.com
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "DelphinusUtils.h"
#include <cstdarg>
#include <cstdio>

using namespace DelphinusUtils;

DelphinusLogLevel delphinusLogLevels [MAX_LOG_MODULES] =
{
    LOG_INFO,
    LOG_INFO,
    LOG_INFO,
    LOG_INFO
};

void DelphinusUtils::LogOutput(uint8_t module, DelphinusLogLevel level, const char* fmt, ...)
{
    va_list argPtr;
    if (level <= LOG_WARN)
    {
        va_start(argPtr, fmt);
        vfprintf(stderr, fmt, argPtr);
        va_end(argPtr);
    }
    else if (level <= delphinusLogLevels[module])
    {
        va_start(argPtr, fmt);
        vfprintf(stdout, fmt, argPtr);
        va_end(argPtr);
    }
}
