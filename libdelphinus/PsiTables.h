/*
 *  PsiTables.h - declaration of program specific information related tables
 *  and sections as specified in the ISO 13818-1 document
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

#ifndef DELPHINUS_PSI_TABLES_H
#define DELPHINUS_PSI_TABLES_H
#include <inttypes.h>
#include <list>

// Table ID                     8
// section syntax indicator     1
// 0                            1
// reserved                     2
// section length               12
// Table ID Extension           16
// reserved                     2
// Version Number               5
// Current Next indicator       1
// Section number               8
// Last section number          8
// Data

class PsiSection
{
    public:
        enum
        {
            TABLE_PAT                        = 0x00,
            TABLE_CAT                        = 0x01,
            TABLE_PMT                        = 0x02,
            TABLE_TSDT                       = 0x03,
            TABLE_SCENE_DESCRIPTION_SECTION  = 0x04,
            TABLE_OBJECT_DESCRIPTION_SECTION = 0x05,
            TABLE_ISO_13818_1_RESERVED_START = 0x06,
            TABLE_ISO_13818_1_RESERVED_END   = 0x37,
            TABLE_ISO_13818_6_START          = 0x38,
            TABLE_ISO_13818_6_END            = 0x3F,
            TABLE_USER_PRIVATE_START         = 0x40,
            TABLE_USER_PRIVATE_END           = 0xFE,
            TABLE_FORBIDDEN                  = 0xFF
        };

    private:
        struct PsiSectionHeader
        {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
            uint8_t byte4;
            uint8_t byte5;
            uint8_t byte6;
            uint8_t byte7;
        };
        uint8_t* start;
        uint8_t pointerField;

    public:
        PsiSection();
        ~PsiSection();

        //Note: the first byte to parse should be the pointer field
        //i.e. the first byte of the payload
        bool parse(uint8_t* data);
        uint8_t getPointerField();
        uint8_t getTableId();
        uint16_t getLength();
        uint16_t getTableIdExtension();
        uint8_t getVersionNumber();
        bool getCurrentNextIndicator();
        uint8_t getSectionNumber();
        uint8_t getLastSectionNumber();
        uint8_t* getData();

        friend class PatSection;
};

class PatSection
{
    public:
        struct ProgramInfo
        {
            uint16_t programNumber;
            uint16_t pmtPid;
        };
        typedef std::list<ProgramInfo> ProgramList;
    private:
        struct PatProgramInfo
        {
            uint8_t byte0;
            uint8_t byte1;
            uint8_t byte2;
            uint8_t byte3;
        };

        bool isComplete;
        ProgramList programList;
        uint16_t sectionLength;
        uint16_t transportStreamId;
        uint16_t networkPid;
        uint8_t* start;
        uint16_t validSize;
        uint8_t currentSection;
        uint8_t lastSection;

    public:
        PatSection();
        ~PatSection();

        void clear();
        void parse(uint8_t* data, uint16_t size);
        void append(uint8_t* data, uint16_t size);
        bool isCompleteSection();

        uint16_t getTransportStreamId();
        const ProgramList& getPrograms();
        uint16_t getNetworkPid();
};

class CatSection
{
    private:
        uint8_t* start;

    public:
        CatSection();
        ~CatSection();

        void parse(uint8_t* data);
        uint8_t* getDescriptor();
};

// FIXME: All descriptors should be returned in a ptr,length fashion

class PmtSection
{
    public:
        enum
        {
            STREAM_TYPE_RESERVED                       = 0x00,
            STREAM_TYPE_11172_VIDEO                    = 0x01,
            STREAM_TYPE_13818_2_VIDEO                  = 0x02,
            STREAM_TYPE_11172_AUDIO                    = 0x03,
            STREAM_TYPE_13818_3_AUDIO                  = 0x04,
            STREAM_TYPE_13818_1_PRIVATE_SECTIONS       = 0x05,
            STREAM_TYPE_13818_1_PES_PRIVATE_DATA       = 0x06,
            STREAM_TYPE_13522_MHEG                     = 0x07,
            STREAM_TYPE_13818_1_DSMCC                  = 0x08,
            STREAM_TYPE_H222_1                         = 0x09,
            STREAM_TYPE_13818_6_TYPE_A                 = 0x0A,
            STREAM_TYPE_13818_6_TYPE_B                 = 0x0B,
            STREAM_TYPE_13818_6_TYPE_C                 = 0x0C,
            STREAM_TYPE_13818_6_TYPE_D                 = 0x0D,
            STREAM_TYPE_13818_1_AUX                    = 0x0E,
            STREAM_TYPE_13818_7_AAC_ADTS               = 0x0F,
            STREAM_TYPE_14496_2_VISUAL                 = 0x10,
            STREAM_TYPE_14496_3_AAC_LATM               = 0x11,
            STREAM_TYPE_14496_1_FLEXMUX_PES            = 0x12,
            STREAM_TYPE_14496_1_FLEXMUX_SECTIONS       = 0x13,
            STREAM_TYPE_13818_6_SYNC_DOWNLOAD_PROTOCOL = 0x14,
            STREAM_TYPE_13818_1_RESERVED_START         = 0x15,
            STREAM_TYPE_13818_1_RESERVED_END           = 0x7F,
            STREAM_TYPE_USER_PRIVATE_START             = 0x80,
            STREAM_TYPE_USER_PRIVATE_END               = 0xFF
        };
        struct StreamInfo
        {
            uint8_t streamType;
            uint16_t pid;
            uint8_t* descriptor;
        };
        typedef std::list<StreamInfo> StreamList;

    private:
        uint8_t* start;

    public:
        PmtSection();
        ~PmtSection();

        void parse(uint8_t* data);
        uint16_t getProgramNumber();
        uint16_t getPcrPid();
        uint8_t* getProgramInfoDescriptor();
        void getStreamList(StreamList& streamList);
};

#define PSI_SSI_SHIFT                   7
#define PSI_HARD_ZERO_MASK              0x40
#define PSI_HARD_ZERO_SHIFT             6
#define PSI_LENGTH_MASK                 0x0F
#define PSI_LENGTH_SHIFT                8
#define PSI_TABLE_ID_EXTN_SHIFT         8
#define PSI_VERSION_MASK                0x3E
#define PSI_VERSION_SHIFT               1
#define PSI_CURR_NEXT_MASK              0x01

#define PSI_GET_TABLE_ID(x)             (x->byte0)
#define PSI_GET_SSI(x)                  (x->byte1 >> PSI_SSI_SHIFT)
#define PSI_GET_HARD_ZERO(x)            ((x->byte1 & PSI_HARD_ZERO_MASK) >> PSI_HARD_ZERO_SHIFT)
#define PSI_GET_LENGTH(x)               (((x->byte1 & PSI_LENGTH_MASK) << PSI_LENGTH_SHIFT) |\
                                         (x->byte2))
#define PSI_GET_TABLE_ID_EXTN(x)        (((x->byte3) << PSI_TABLE_ID_EXTN_SHIFT) | (x->byte4))
#define PSI_GET_VERSION(x)              ((x->byte5 & PSI_VERSION_MASK) >> PSI_VERSION_SHIFT)
#define PSI_GET_CURR_NEXT(x)            ((x->byte5) & PSI_CURR_NEXT_MASK)
#define PSI_GET_SECTION_NUMBER(x)       (x->byte6)
#define PSI_GET_LAST_SECTION_NUMBER(x)  (x->byte7)

#define PSI_HEADER_START                ((PsiSection::PsiSectionHeader*)(start))

#define PAT_PROG_NUMBER_SHIFT           8
#define PAT_PID_MASK                    0x1FFF
#define PAT_PID_SHIFT                   8

#define PAT_GET_PROG_NUMBER(x)          (((x->byte0) << PAT_PROG_NUMBER_SHIFT) | (x->byte1))
#define PAT_GET_PID(x)                  (((x->byte2 << PAT_PROG_NUMBER_SHIFT) | (x->byte3)) & PAT_PID_MASK)

inline uint8_t PsiSection::getPointerField()
{
    return pointerField;
}

inline uint8_t PsiSection::getTableId()
{
    return PSI_GET_TABLE_ID(PSI_HEADER_START);
}

inline uint16_t PsiSection::getLength()
{
    return PSI_GET_LENGTH(PSI_HEADER_START);
}

inline uint16_t PsiSection::getTableIdExtension()
{
    return PSI_GET_TABLE_ID_EXTN(PSI_HEADER_START);
}

inline uint8_t PsiSection::getVersionNumber()
{
    return PSI_GET_VERSION(PSI_HEADER_START);
}

inline bool PsiSection::getCurrentNextIndicator()
{
    return PSI_GET_CURR_NEXT(PSI_HEADER_START);
}

inline uint8_t PsiSection::getSectionNumber()
{
    return PSI_GET_SECTION_NUMBER(PSI_HEADER_START);
}

inline uint8_t PsiSection::getLastSectionNumber()
{
    return PSI_GET_LAST_SECTION_NUMBER(PSI_HEADER_START);
}

inline uint8_t* PsiSection::getData()
{
    return start + sizeof(struct PsiSectionHeader);
}

inline bool PatSection::isCompleteSection()
{
    return isComplete;
}

inline uint16_t PatSection::getTransportStreamId()
{
    return transportStreamId;
}

inline const PatSection::ProgramList& PatSection::getPrograms()
{
    return programList;
}

inline uint16_t PatSection::getNetworkPid()
{
    return networkPid;
}


#endif
