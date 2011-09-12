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
#include <inttypes.h>

class PesPacket
{
    public:
        enum {
            STREAM_ID_PROGRAM_STREAM_MAP = 0xBC,
            STREAM_ID_PRIVATE_1 = 0xBD,
            STREAM_ID_PADDING = 0xBE,
            STREAM_ID_PRIVATE_2 = 0xBF,
            STREAM_ID_AUDIO_START = 0xC0,
            STREAM_ID_AUDIO_END = 0xDF,
            STREAM_ID_VIDEO_START = 0xE0,
            STREAM_ID_VIDEO_END = 0xEF,
            STREAM_ID_ECM = 0xF0,
            STREAM_ID_EMM = 0xF1,
            //TODO: find a better name
            STREAM_ID_DSMCC_STREAM = 0xF2,
            STREAM_ID_ISO_IEC_13522_STREAM = 0xF3,
            STREAM_ID_H222_1_TYPE_A = 0xF4,
            STREAM_ID_H222_1_TYPE_B = 0xF5,
            STREAM_ID_H222_1_TYPE_C = 0xF6,
            STREAM_ID_H222_1_TYPE_D = 0xF7,
            STREAM_ID_H222_1_TYPE_E = 0xF8,
            STREAM_ID_ANCILLARY = 0xF9,
            STREAM_ID_SL_PACKETIZED = 0xFA,
            STREAM_ID_FLEXMUX = 0xFB,
            STREAM_ID_PROGRAM_STREAM_DIR = 0xFF
        };
    private:

    public:
        void clear();
        void appendPartialPes(uint8_t* data);
        void parse(uint8_t* data);

        bool isValid();
        uint32_t getLength();
        uint32_t getStartCodePrefix();
        uint8_t getStreamId();

};

class TransportStreamPacket
{
    public:
        enum { PACKET_SIZE = 188 };

    private:
        struct TransportStreamHeader
        {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        };
        uint8_t* start;

    public:
        bool parse(uint8_t* data);
        uint8_t* getStart();
        bool isValid();

        uint8_t getSyncByte();
        bool getTransportErrorIndicator();
        bool getPayloadUnitStartIndicator();
        bool getTransportPriority();
        uint16_t getPid();
        uint8_t getTransportScramblingControl();
        uint8_t getAdaptationFieldControl();
        bool hasAdaptationField();
        bool hasData();
        uint8_t getContinuityCounter();
        uint8_t* getAdaptationField();
        uint8_t* getPayload();
};

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

#define TS_HEADER_START ((TransportStreamHeader*)start)

inline bool TransportStreamPacket::parse(uint8_t* data)
{
    start = data;
    return isValid();
}

inline uint8_t* TransportStreamPacket::getStart()
{
    return start;
}

inline bool TransportStreamPacket::isValid()
{
    return getSyncByte() == 0x47;
}

inline uint8_t TransportStreamPacket::getSyncByte()
{
    return TS_GET_SYNC_BYTE(TS_HEADER_START);
}

inline bool TransportStreamPacket::getTransportErrorIndicator()
{
    return TS_GET_TEI(TS_HEADER_START);
}

inline bool TransportStreamPacket::getPayloadUnitStartIndicator()
{
    return TS_GET_PUSI(TS_HEADER_START);
}

inline bool TransportStreamPacket::getTransportPriority()
{
    return TS_GET_TP(TS_HEADER_START);
}

inline uint16_t TransportStreamPacket::getPid()
{
    return TS_GET_PID(TS_HEADER_START);
}

inline uint8_t TransportStreamPacket::getTransportScramblingControl()
{
    return TS_GET_TSC(TS_HEADER_START);
}

inline uint8_t TransportStreamPacket::getAdaptationFieldControl()
{
    return TS_GET_AFC(TS_HEADER_START);
}

inline bool TransportStreamPacket::hasAdaptationField()
{
    return getAdaptationFieldControl() & 0x02;
}

inline bool TransportStreamPacket::hasData()
{
    return getAdaptationFieldControl() & 0x01;
}

inline uint8_t TransportStreamPacket::getContinuityCounter()
{
    return TS_GET_CC(TS_HEADER_START);
}

#endif
