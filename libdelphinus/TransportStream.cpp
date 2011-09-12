/*
 *  TransportStream.cpp - declaration of types related to transport stream header
 *  and the fields within as specified in the ISO 13818-1 document
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

#include "TransportStream.h"
#include <cstddef>

TransportStreamPacket::TransportStreamPacket()
{
}

TransportStreamPacket::~TransportStreamPacket()
{
    if (isMemoryAllocated)
    {
        delete start;
    }
}

bool TransportStreamPacket::parse(uint8_t* data, uint64_t size)
{
    if (isMemoryAllocated && start)
    {
        delete start;
        isMemoryAllocated = false;
    }
    start = data;
    startOffset = 0;
    packetSize = 0;
    isValid = false;

    if (size < PACKET_SIZE_TS)
    {
        // We need at least one whole TS packet
        isValid = false;
    }
    else
    {
        if (getSyncByte() == 0x47)
        {
            packetSize = PACKET_SIZE_TS;
            isValid = true;
        }
        else
        {
            // Check if it is TTS
            startOffset = 4;
            if (getSyncByte() != 0x47)
            {
                isValid = false;
            }
            else
            {
                packetSize = PACKET_SIZE_TTS;
                isValid = true;
            }
        }
    }
    return isValid;
}

TransportStreamPacket* TransportStreamPacket::copy()
{
    if (start && isValid)
    {
        TransportStreamPacket* tsPacket = new TransportStreamPacket();
        uint8_t* copiedData = new uint8_t[packetSize];
        tsPacket->parse(copiedData, packetSize);
        tsPacket->isMemoryAllocated = true;
        return tsPacket;
    }
    else
    {
        return NULL;
    }
}

