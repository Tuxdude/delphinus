/*
 *  DelphinusUtils.h - Common set of types and helper functions used by the
 *  rest of the delphinus project.
 *
 *  This file is part of delphinus - a stream analyzer for various multimedia
 *  stream formats and containers.
 *
 *  Copyright (C) 2012 Ash (Tuxdude)
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

/**
 *  \file DelphinusUtils.h
 *  \brief Set of utilities and helper methods.
 *
 *  Contains types, utility and helper methods that is used by the rest of
 *  the Delphinus project.
 */

#ifndef DELPHINUS_DELPHINUS_UTILS_H
#define DELPHINUS_DELPHINUS_UTILS_H

#ifdef DELPHINUS_IN_COMMON_DIR
#define DELPHINUS_UTILS_EXTERN
#else
#define DELPHINUS_UTILS_EXTERN extern
#endif

#include <inttypes.h>

/**
 *  \brief  This namespace contains the various types, utilities and helper
 *          methods used by the rest of the Delphinus project.
 */
namespace DelphinusUtils
{

/**
 *
 *  \brief A structure for accessing a binary data structure at byte level.
 *  
 *  A byte field structure useful for accessing the individual bytes
 *  of a binary data structure. Individual bits than can be accessed by mask
 *  and shift operations thereby providing an endianness portable way of
 *  accessing bit fields.
 */
    struct ByteField
    {
/** Byte 0 */
        uint8_t byte0;
/** Byte 1 */
        uint8_t byte1;
/** Byte 2 */
        uint8_t byte2;
/** Byte 3 */
        uint8_t byte3;
/** Byte 4 */
        uint8_t byte4;
/** Byte 5 */
        uint8_t byte5;
/** Byte 6 */
        uint8_t byte6;
/** Byte 7 */
        uint8_t byte7;
/** Byte 8 */
        uint8_t byte8;
/** Byte 9 */
        uint8_t byte9;
    };

/**
 *
 *  \brief Enum for various log levels.
 *    
 *  The logs can be tagged with different levels which decide the stream in
 *  which the logs go as well as well as conditionally ignoring logs from
 *  different levels like LOG_DEBUG when it is not required. Refer
 *  DelphinusUtils::LogOutput() for more details.
 */
    enum DelphinusLogLevel
    {
/** Log Level Error */
        LOG_ERROR = 0,
/** Log Level Warning */
        LOG_WARN  = 1,
/** Log Level Information */
        LOG_INFO  = 2,
/** Log Level Debug */
        LOG_DEBUG = 3
    };

/** Maximum number of log modules */
    enum MaxModules
    {
/** Maximum number of log modules */
        MAX_LOG_MODULES = 32
    };

/** Log levels for all the modules */
    DELPHINUS_UTILS_EXTERN DelphinusLogLevel delphinusLogLevels [MAX_LOG_MODULES];

/**
 *  \brief  Log the given message to the output (stdout/stderr).
 *  \param  module The module from which this log is sent
 *  \param  level The log level designated.
 *  \param  fmt Format String
 *  \param  ... Parameters (variable arguments) for the format string
 */
    void LogOutput(uint8_t module, DelphinusLogLevel level, const char* fmt, ...);
}


#endif
