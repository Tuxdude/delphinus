/*
 *  Pes.h - declaration of types related to packetized elementary stream header
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

#ifndef DELPHINUS_PES_H
#define DELPHINUS_PES_H
#include "common/DelphinusUtils.h"

class PesPacket
{
    private:
        bool isPes;
        uint8_t* start;

    public:
        PesPacket();
        ~PesPacket();

        void clear();
        // Always call parse to start the PES parsing
        // It creates new memory for the parsed data and copies
        // it if it seems to be PES
        void parse(uint8_t* data);
        // append the pes data to an already initialized Pes object
        // which contains the payload preceding the current one
        void appendPartialPes(uint8_t* data);

        bool isValid();
        uint16_t getLength();
        uint32_t getStartCodePrefix();
        uint8_t getStreamId();

};

#define PES_START_CODE_SHIFT_0          16
#define PES_START_CODE_SHIFT_1          8
#define PES_LENGTH_SHIFT                8              

#define PES_GET_START_CODE(x)           ((x->byte0 << PES_START_CODE_SHIFT_0) |\
                                         (x->byte1 << PES_START_CODE_SHIFT_1) |\
                                         (x->byte2))
#define PES_GET_STREAM_ID(x)            (x->byte3)
#define PES_GET_LENGTH(x)               ((x->byte4 << PES_LENGTH_SHIFT) |\
                                         (x->byte5))

#define PES_HEADER_START ((ByteField*)(start))

inline bool PesPacket::isValid()
{
    return isPes;
}

inline uint16_t PesPacket::getLength()
{
    return PES_GET_LENGTH(PES_HEADER_START);
}

inline uint32_t PesPacket::getStartCodePrefix()
{
    return PES_GET_START_CODE(PES_HEADER_START);
}

inline uint8_t PesPacket::getStreamId()
{
    return PES_GET_STREAM_ID(PES_HEADER_START);
}

#endif
