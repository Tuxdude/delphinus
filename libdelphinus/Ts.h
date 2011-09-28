/*
 *  Ts.h - declaration of types related to transport stream header
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

/**
 *  \file   Ts.h
 *  \brief  Transport Stream packet related types.
 *
 *  Contains all the data structures, types and methods for handling a
 *  Transport Stream (TS) packet and the fields within.
 *
 */

#ifndef DELPHINUS_TS_H
#define DELPHINUS_TS_H

#include "common/DelphinusUtils.h"
#include "MpegConstants.h"

/**
 *  \brief  TsPacket represents a single TS packet.
 *
 *  TsPacket provides the means to access the various fields in the Transport
 *  Stream packet header as well as the adaptation field and the payload.
 */

class TsPacket
{
    private:
        uint8_t* start;
        uint8_t startOffset;
        uint8_t packetSize;
        uint8_t adaptationFieldOffset;
        uint16_t payloadOffset;
        bool isMemoryAllocated;
        bool isValid;

    public:
        TsPacket();
        ~TsPacket();

/**
 *  \brief  Parse the given set of bytes as a TS Packet and return the status.
 *  \param  data The starting address of data to parse.
 *  \param  size Maximum size to parse.
 *  \return Returns true if the parse is successfull and is a valid TS packet.
 */
        bool parse(uint8_t* data, uint64_t size);
/**
 *  \brief  Get the start address of the TS Packet.
 *  \return Returns the start address of the TS Packet.
 */
        uint8_t* getStart();
/**
 *  \brief  Get the size of the TS Packet.
 *  \return TS Packet Size.
 */
        uint8_t getPacketSize();
/**
 *  \brief  Make a copy of the TS Packet. The memory is allocated from the heap
 *          for the new packet and is automatically freed in the destructor of
 *          TsPacket.
 *  \return TsPacket handle with reference to a copy of the original data.
 */
        TsPacket* copy();
/**
 *  \brief  Get the SYNC byte field in the TS Packet header.
 *  \return SYNC byte in the TS Packet Header.
 */
        uint8_t getSyncByte();
/**
 *  \brief  Get the Transport Error Indicator flag in the TS Packet header.
 *  \return Transport Error Indicator flag in the TS Packet header.
 */
        bool getTransportErrorIndicator();
/**
 *  \brief  Get the Payload Unit Start Indicator flag in the TS Packet header.
 *  \return Payload Unit Start Indicator flag in the TS Packet header.
 */
        bool getPayloadUnitStartIndicator();
/**
 *  \brief  Get the Transport Priority flag in the TS Packet header.
 *  \return Transport Priority flag in the TS Packet header.
 */
        bool getTransportPriority();
/**
 *  \brief  Get the PID of the TS Packet.
 *  \return 13-bit PID field in the TS Packet header.
 */
        uint16_t getPid();
/**
 *  \brief  Get the Transport Scrambling Control flags in the TS Packet header.
 *  \return 2-bit Transport Scrambling Control flags in the TS Packet header.
 */
        uint8_t getTransportScramblingControl();
/**
 *  \brief  Get the Adaptation Field Control flags in the TS Packet header.
 *  \return 2-bit Adaptation Field Control flags in the TS Packet header.
 */
        uint8_t getAdaptationFieldControl();
/**
 *  \brief  Determine if the TS Packet carries an adaptation field.
 *  \return TS Packet carries an adaptation field or not.
 */
        bool hasAdaptationField();
/**
 *  \brief  Determine if the TS Packet carries a payload.
 *  \return TS Packet carries a payload or not.
 */
        bool hasPayload();
/**
 *  \brief  Get the Continuity Counter field in the TS Packet header.
 *  \return 4-bit Continuity Counter in the TS Packet header.
 */
        uint8_t getContinuityCounter();
/**
 *  \brief  Get the start of the Adaptation Field in the TS Packet.
 *  \return Address of the start of the Adaptation Field in the TS Packet.
 */
        uint8_t* getAdaptationField();
/**
 *  \brief  Get the start offset of the payload in the TS packet.
 *  \return Start offset of the payload from the beginning of the TS packet.
 */
        uint8_t getPayloadOffset();
/**
 *  \brief  Get the size of the payload in the TS Packet.
 *  \return Size of the payload in the TS Packet.
 */
        uint8_t getPayloadSize();
/**
 *  \brief  Get the start of the payload in the TS Packet.
 *  \return Address of the start of the payload in the TS Packet.
 */
        uint8_t* getPayload();
};

/**
 *  \brief  AdaptationField represents the adaptation field within a TS Packet.
 *
 *  AdaptationField provides the means to access the various fields within the
 *  adaptation field of a TS Packet.
 */
class AdaptationField
{
    private:
        uint8_t* start;
        uint8_t length;
        uint8_t* pcrStart;
        uint8_t* opcrStart;
        uint8_t* spliceCountdownStart;
        uint8_t* privateDataStart;
        uint8_t* adaptationFieldExtensionStart;

    public:
        void parse(uint8_t* data);
        uint8_t* getStart();
        uint8_t getLength();
        bool getDiscontinuityIndicator();
        bool getRandomAccessIndicator();
        bool getEsPriorityIndicator();
        bool hasPcr();
        void getPcr(uint64_t& pcrBase, uint8_t pcrExtn);
        bool hasOpcr();
        void getOpcr(uint64_t& opcrBase, uint8_t opcrExtn);
        bool hasSpliceCountdown();
        int8_t getSpliceCountdown();
        bool hasTransportPrivateData();
        void getPrivateData(uint8_t*& privateDataStart, uint8_t& length);
        uint8_t* getAdaptationFieldExtension();
};

/**
 *  \brief  AdaptationFieldExtension represents the adaptation field extension
 *  over the regular adaptation field.
 *
 *  AdaptationField provides the means to access the various fields within the
 *  adaptation field extension of a TS Packet.
 */

class AdaptationFieldExtension
{
    private:
        uint8_t* start;
        uint8_t length;
        uint8_t* ltwStart;
        uint8_t* piecewiseRateStart;
        uint8_t* seamlessSpliceStart;

    public:
        void parse(uint8_t* data);
        uint8_t* getStart();
        uint8_t getLength();
        bool hasLtw();
        void getLtw(bool& isValid, uint16_t& offset);
        bool hasPiecewiseRate();
        uint32_t getPiecewiseRate();
        bool hasSeamlessSplice();
        void getSeamlessSpliceInfo(uint8_t& spliceType, uint64_t& dtsNextAu);
};


#define TS_TEI_MASK                     0x80
#define TS_TEI_SHIFT                    7
#define TS_PUSI_MASK                    0x40
#define TS_PUSI_SHIFT                   6
#define TS_TP_MASK                      0x20
#define TS_TP_SHIFT                     5
#define TS_PID_HIGH_MASK                0x01F
#define TS_PID_HIGH_SHIFT               8
#define TS_TSC_MASK                     0xC0
#define TS_TSC_SHIFT                    6
#define TS_AFC_MASK                     0x30
#define TS_AFC_SHIFT                    4
#define TS_CC_MASK                      0x0F

#define TS_GET_SYNC_BYTE(x)             (x->byte0)
#define TS_GET_TEI(x)                   ((x->byte1 & TS_TEI_MASK) >> TS_TEI_SHIFT)
#define TS_GET_PUSI(x)                  ((x->byte1 & TS_PUSI_MASK) >> TS_PUSI_SHIFT)
#define TS_GET_TP(x)                    ((x->byte1 & TS_TP_MASK) >> TS_TP_SHIFT)
#define TS_GET_PID(x)                   (((x->byte1 & TS_PID_HIGH_MASK) << TS_PID_HIGH_SHIFT) | x->byte2)
#define TS_GET_TSC(x)                   ((x->byte3 & TS_TSC_MASK) >> TS_TSC_SHIFT)
#define TS_GET_AFC(x)                   ((x->byte3 & TS_AFC_MASK) >> TS_AFC_SHIFT)
#define TS_GET_CC(x)                    (x->byte3 & TS_CC_MASK)

#define TS_HEADER_START                 ((ByteField*)(start + startOffset))

inline uint8_t* TsPacket::getStart()
{
    return start;
}

inline uint8_t TsPacket::getPacketSize()
{
    return packetSize;
}

inline uint8_t TsPacket::getSyncByte()
{
    return TS_GET_SYNC_BYTE(TS_HEADER_START);
}

inline bool TsPacket::getTransportErrorIndicator()
{
    return TS_GET_TEI(TS_HEADER_START);
}

inline bool TsPacket::getPayloadUnitStartIndicator()
{
    return TS_GET_PUSI(TS_HEADER_START);
}

inline bool TsPacket::getTransportPriority()
{
    return TS_GET_TP(TS_HEADER_START);
}

inline uint16_t TsPacket::getPid()
{
    return TS_GET_PID(TS_HEADER_START);
}

inline uint8_t TsPacket::getTransportScramblingControl()
{
    return TS_GET_TSC(TS_HEADER_START);
}

inline uint8_t TsPacket::getAdaptationFieldControl()
{
    return TS_GET_AFC(TS_HEADER_START);
}

inline bool TsPacket::hasAdaptationField()
{
    return getAdaptationFieldControl() & 0x02;
}

inline bool TsPacket::hasPayload()
{
    return getAdaptationFieldControl() & 0x01;
}

inline uint8_t TsPacket::getContinuityCounter()
{
    return TS_GET_CC(TS_HEADER_START);
}

inline uint8_t* TsPacket::getAdaptationField()
{
    return (start + adaptationFieldOffset);
}

inline uint8_t* TsPacket::getPayload()
{
    return (start + payloadOffset);
}

inline uint8_t TsPacket::getPayloadOffset()
{
    return payloadOffset;
}

inline uint8_t TsPacket::getPayloadSize()
{
    return packetSize - payloadOffset;
}
#endif
