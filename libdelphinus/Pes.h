/*
 *  Pes.h - declaration of types related to packetized elementary stream header
 *  and the fields within as specified in the ISO 13818-1 document
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

/**
 *  \file   Pes.h
 *  \brief  Packetized Elementary Stream related types.
 *
 *  Contains all the data structures, types and methods for handling
 *  Packetized Elementary Stream (PES) and the fields within.
 */

#ifndef DELPHINUS_PES_H
#define DELPHINUS_PES_H
#include "common/DelphinusUtils.h"

/**
 *  \brief  PesPacket represents a single PES
 *
 *  PesPacket provides the means to access the various fields in the PES
 *  header as well as the payload.
 */
class PesPacket
{
    private:
        bool isPes;
        uint8_t* start;

    public:
        PesPacket();
        ~PesPacket();

/**
 *  \brief  Clear the current PesPacket handle. Memory is automatically
 *          dealloacted if it was allocated earlier for storing the data.
 */
        void clear();
        // Always call parse to start the PES parsing
        // It creates new memory for the parsed data and copies
        // it if it seems to be PES
/**
 *  \brief  Start parsing a new PES. Verifies if it is the start of a valid
 *          PES, allocates memory on the heap based on the PES length field,
 *          and copies the bytes from data to the newly allocated memory
 *          region, so the data gets persisted into a separate memory region.
 *          This method should be used when a new PES needs to be parsed and
 *          even if the complete PES is not available immediately. If memory
 *          was previously allocated for the data in the PesPacket handle it
 *          would be deallocated first before allocating memory based on the
 *          current PES length.
 */
        void parse(uint8_t* data);
/**
 *  \brief  Append the subsequent bytes of the PES to an existing partial PES
 *          which the PesPacket handle currently represents. appendPartialPes()
 *          should be called only after parse().
 */
        void appendPartialPes(uint8_t* data);
/**
 *  \brief  Determine if it is a valid complete/partial PES. To check if the
 *          PES is complete use isComplete() instead.
 *  \return Valid complete/partial PES.
 */
        bool isValid();
/**
 *  \brief  Determine if it is a complete PES. To check the validity of a
 *          complete/partial PES use isValid() instead.
 *  \return Valid complete/partial PES.
 */
        bool isComplete();
/**
 *  \brief  Get the length field in the PES header.
 *  \return Length field in the PES header.
 */
        uint16_t getLength();
/**
 *  \brief  Get the start code prefix of the PES.
 *  \return Start code prefix of the PES.
 */
        uint32_t getStartCodePrefix();
/**
 *  \brief  Get the Stream ID field in the PES header.
 *  \return Stream ID field in the PES header.
 */
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

#define PES_HEADER_START ((DelphinusUtils::ByteField*)(start))

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
