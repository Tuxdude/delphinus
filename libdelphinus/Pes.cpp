/*
 *  Pes.cpp - declaration of types related to packetized elementary stream header
 *  and the fields within as specified in the ISO 13818-1 document
 *
 *  This file is part of delphinus.
 *
 *  Copyright (C) 2012 Ash (Tuxdude) <tuxdude.github@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program.  If not, see
 *  <http://www.gnu.org/licenses/>.
 */

#include <cstddef>
#include "Pes.h"

PesPacket::PesPacket()
    :   isPes(false),
        start(NULL)
{
}

PesPacket::~PesPacket()
{
    clear();
}

void PesPacket::clear()
{
    if (start)
    {
//        delete start;
        start = NULL;
    }
    isPes = false;
}

void PesPacket::appendPartialPes(uint8_t* data)
{
    (void)data;
}

void PesPacket::parse(uint8_t* data)
{
    clear();
    start = data;
    //FIXME
    isPes = true;
}

