/*
 *  DelphinusUtils.h - Common set of types and helper functions used by the
 *  rest of the delphinus project.
 *
 *  This file is part of delphinus - a stream analyzer for various multimedia
 *  stream formats and containers.
 *
 *  Copyright (C) 2011 Ashwin (Tuxdude)
 *  tuxdude@users.sourceforge.net
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

#include <inttypes.h>

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
/**
 * \brief Byte 0
 */
    uint8_t byte0;
/**
 * \brief Byte 1
 */
    uint8_t byte1;
/**
 * \brief Byte 2
 */
    uint8_t byte2;
/**
 * \brief Byte 3
 */
    uint8_t byte3;
/**
 * \brief Byte 4
 */
    uint8_t byte4;
/**
 * \brief Byte 5
 */
    uint8_t byte5;
/**
 * \brief Byte 6
 */
    uint8_t byte6;
/**
 * \brief Byte 7
 */
    uint8_t byte7;
/**
 * \brief Byte 8
 */
    uint8_t byte8;
/**
 * \brief Byte 9
 */
    uint8_t byte9;
};

#endif
