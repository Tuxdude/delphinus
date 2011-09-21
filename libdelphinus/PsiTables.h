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
#include "common/DelphinusUtils.h"
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

struct PsiDescriptor
{
    uint8_t* start;
    uint16_t size;
};

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
        uint8_t* start;
        uint8_t pointerField;

    public:
        PsiSection();
        ~PsiSection();

        //Note: the first byte to parse should be the pointer field
        //i.e. the first byte of the payload
        bool parse(uint8_t* data);
        uint8_t getPointerField();
        uint8_t getDataOffset();
        uint8_t getTableId();
        uint16_t getLength();
        uint16_t getTableIdExtension();
        uint8_t getVersionNumber();
        bool getCurrentNextIndicator();
        uint8_t getSectionNumber();
        uint8_t getLastSectionNumber();
        uint8_t* getData();

        friend class PsiSectionCommon;
};

class PsiSectionCommon
{
    protected:
        bool isComplete;
        uint8_t* start;
        uint16_t sectionLength;
        uint16_t tableIdExtension;
        uint16_t validSize;
        uint8_t currentSection;
        uint8_t lastSection;

        PsiSectionCommon();
        virtual ~PsiSectionCommon();
        virtual void onComplete() = 0;
        void parse(uint8_t* data, uint16_t size, uint8_t tableId);
        void append(uint8_t* data, uint16_t size, uint8_t tableId);

    public:
        void clear();
        bool isCompleteSection();
};

class PatSection : public PsiSectionCommon
{
    public:
        struct ProgramInfo
        {
            uint16_t programNumber;
            uint16_t pmtPid;
        };
        typedef std::list<ProgramInfo> ProgramList;

    private:
        ProgramList programList;
        uint16_t networkPid;

        void onComplete();

    public:
        PatSection();
        ~PatSection();

        void parse(uint8_t* data, uint16_t size);
        void append(uint8_t* data, uint16_t size);

        uint16_t getTransportStreamId();
        const ProgramList& getPrograms();
        uint16_t getNetworkPid();
};

class CatSection : public PsiSectionCommon
{
    private:
        uint8_t* start;
        PsiDescriptor descriptor;

        void onComplete();

    public:
        CatSection();
        ~CatSection();

        void parse(uint8_t* data, uint16_t size);
        void append(uint8_t* data, uint16_t size);

        const PsiDescriptor& getDescriptor();
};

// FIXME: All descriptors should be returned in a ptr,length fashion

class PmtSection : public PsiSectionCommon
{
    public:
        enum
        {
            STREAM_TYPE_RESERVED                        = 0x00,
            STREAM_TYPE_11172_VIDEO                     = 0x01,
            STREAM_TYPE_13818_2_VIDEO                   = 0x02,
            STREAM_TYPE_11172_AUDIO                     = 0x03,
            STREAM_TYPE_13818_3_AUDIO                   = 0x04,
            STREAM_TYPE_13818_1_PRIVATE_SECTIONS        = 0x05,
            STREAM_TYPE_13818_1_PES_PRIVATE_DATA        = 0x06,
            STREAM_TYPE_13522_MHEG                      = 0x07,
            STREAM_TYPE_13818_1_DSMCC                   = 0x08,
            STREAM_TYPE_H222_1                          = 0x09,
            STREAM_TYPE_13818_6_TYPE_A                  = 0x0A,
            STREAM_TYPE_13818_6_TYPE_B                  = 0x0B,
            STREAM_TYPE_13818_6_TYPE_C                  = 0x0C,
            STREAM_TYPE_13818_6_TYPE_D                  = 0x0D,
            STREAM_TYPE_13818_1_AUX                     = 0x0E,
            STREAM_TYPE_13818_7_AAC_ADTS                = 0x0F,
            STREAM_TYPE_14496_2_VIDEO                   = 0x10,
            STREAM_TYPE_14496_3_AAC_LATM                = 0x11,
            STREAM_TYPE_14496_1_FLEXMUX_PES             = 0x12,
            STREAM_TYPE_14496_1_FLEXMUX_SECTIONS        = 0x13,
            STREAM_TYPE_13818_6_SYNC_DOWNLOAD_PROTOCOL  = 0x14,
            STREAM_TYPE_14496_10_VIDEO                  = 0x1b,
            // 0x80 can also be LPCM audio
            STREAM_TYPE_DC_II_VIDEO                     = 0x80,
            STREAM_TYPE_A52_AC3_AUDIO                   = 0x81,
            // 0x82 can also be DVD SPU subtitles
            STREAM_TYPE_HDMV_DTS_AUDIO                  = 0x82,
            // 0x83 can be either LPCM or BD TrueHD
            STREAM_TYPE_LPCM_AUDIO                      = 0x83,
            // 0x84 can be either SDDS or E-AC-3 audio
            STREAM_TYPE_SDDS_AUDIO                      = 0x84,
            // 0x85 can also be DTS HD HR Audio
            STREAM_TYPE_ATSC_PROGRAM_ID                 = 0x85,
            STREAM_TYPE_DTS_HD_MASTER_AUDIO             = 0x86,
            STREAM_TYPE_E_AC3_AUDIO                     = 0x87,
            STREAM_TYPE_DTS_AUDIO                       = 0x8A,
            STREAM_TYPE_PGS_SUBTITLE                    = 0x90,
            // 0x91 can also be BD Interactive Graphics Stream
            STREAM_TYPE_A52B_AC3_AUDIO                  = 0x91,
            STREAM_TYPE_DVD_SPU_VLS_SUBTITLE            = 0x92,
            STREAM_TYPE_SDDS_AUDIO_2                    = 0x94,
            STREAM_TYPE_MSCODEC_VIDEO                   = 0xA0,
            // BD E-AC3 secondary audio
            STREAM_TYPE_E_AC3_SECONDARY_AUDIO           = 0xA1,
            // BD DTS-HD secondary audio
            STREAM_TYPE_DTS_HD_SECONDARY_AUDIO          = 0xA2,
            // VC-1 advanced profile
            STREAM_TYPE_VC_1_AP                         = 0xEA,
            // VC-1 Simple and main profile
            STREAM_TYPE_VC_1_SMP                        = 0xEB,
            STREAM_TYPE_13818_1_RESERVED_START          = 0x15,
            STREAM_TYPE_13818_1_RESERVED_END            = 0x7F,
            STREAM_TYPE_USER_PRIVATE_START              = 0x80,
            STREAM_TYPE_USER_PRIVATE_END                = 0xFF
        };
        struct StreamInfo
        {
            uint8_t streamType;
            uint16_t pid;
            PsiDescriptor descriptor;
        };
        typedef std::list<StreamInfo> StreamList;

    private:
        uint16_t pcrPid;
        PsiDescriptor programInfoDescriptor;
        StreamList streamList;

        void onComplete();

    public:
        PmtSection();
        ~PmtSection();

        static const char* getStreamTypeStr(uint8_t streamType);

        void parse(uint8_t* data, uint16_t size);
        void append(uint8_t* data, uint16_t size);

        uint16_t getProgramNumber();
        uint16_t getPcrPid();
        const PsiDescriptor& getProgramInfoDescriptor();
        const StreamList& getStreamList();
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

#define PSI_HEADER_START                ((ByteField*)start)

#define PAT_PROG_NUMBER_SHIFT           8
#define PAT_PID_MASK                    0x1FFF
#define PAT_PID_SHIFT                   8

#define PAT_GET_PROG_NUMBER(x)          (((x->byte0) << PAT_PROG_NUMBER_SHIFT) | (x->byte1))
#define PAT_GET_PID(x)                  (((x->byte2 << PAT_PROG_NUMBER_SHIFT) | (x->byte3)) & PAT_PID_MASK)
#define PAT_PROG_INFO_START             ((ByteField*)(start + offset))

#define PMT_PCR_PID_MASK                0x1FFF
#define PMT_PCR_PID_SHIFT               8
#define PMT_PIL_MASK                    0x0FFF
#define PMT_PIL_SHIFT                   8
#define PMT_ES_PID_MASK                 0x1FFF
#define PMT_ES_PID_SHIFT                8
#define PMT_EIL_MASK                    0x0FFF
#define PMT_EIL_SHIFT                   8

#define PMT_GET_PCR_PID(x)              (((x->byte0 << PMT_PCR_PID_SHIFT) | (x->byte1)) & PMT_PCR_PID_MASK)
#define PMT_GET_PROG_INFO_LENGTH(x)     (((x->byte2 << PMT_PIL_SHIFT) | (x->byte3)) & PMT_PIL_MASK)
#define PMT_GET_STREAM_TYPE(x)          (x->byte0)
#define PMT_GET_ES_PID(x)               (((x->byte1 << PMT_ES_PID_SHIFT) | (x->byte2)) & PMT_ES_PID_MASK)
#define PMT_GET_ES_INFO_LENGTH(x)       (((x->byte3 << PMT_EIL_SHIFT) | (x->byte4)) & PMT_EIL_MASK)


inline uint8_t PsiSection::getPointerField()
{
    return pointerField;
}

inline uint8_t PsiSection::getDataOffset()
{
    return 1 + pointerField;
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
    return start + 8;
}

inline bool PsiSectionCommon::isCompleteSection()
{
    return isComplete;
}

inline void PatSection::parse(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::parse(data, size, PsiSection::TABLE_PAT);
}

inline void PatSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, PsiSection::TABLE_PAT);
}

inline uint16_t PatSection::getTransportStreamId()
{
    return tableIdExtension;
}

inline const PatSection::ProgramList& PatSection::getPrograms()
{
    return programList;
}

inline uint16_t PatSection::getNetworkPid()
{
    return networkPid;
}

inline void PmtSection::parse(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::parse(data, size, PsiSection::TABLE_PMT);
}

inline void PmtSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, PsiSection::TABLE_PMT);
}

inline uint16_t PmtSection::getProgramNumber()
{
    return tableIdExtension;
}

inline uint16_t PmtSection::getPcrPid()
{
    return pcrPid;
}

inline const PsiDescriptor& PmtSection::getProgramInfoDescriptor()
{
    return programInfoDescriptor;
}

inline const PmtSection::StreamList& PmtSection::getStreamList()
{
    return streamList;
}

inline void CatSection::parse(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::parse(data, size, PsiSection::TABLE_CAT);
}

inline void CatSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, PsiSection::TABLE_CAT);
}

inline const PsiDescriptor& CatSection::getDescriptor()
{
    return descriptor;
}

#endif
