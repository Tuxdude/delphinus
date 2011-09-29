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

/**
 *  \file   PsiTables.h
 *  \brief  Types and data structures for parsing PSI and other tables
 *          transmitted within sections.
 *
 *  Contains data structures, types and helper methods to parse sections as
 *  well as standard PSI tables found within a Transport Stream (TS).
 */

#ifndef DELPHINUS_PSI_TABLES_H
#define DELPHINUS_PSI_TABLES_H

#include <list>
#include "common/DelphinusUtils.h"
#include "MpegConstants.h"

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

class PmtSection : public PsiSectionCommon
{
    public:
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

class TsdtSection : public PsiSectionCommon
{
    private:
        uint8_t* start;
        PsiDescriptor descriptor;

        void onComplete();

    public:
        TsdtSection();
        ~TsdtSection();

        void parse(uint8_t* data, uint16_t size);
        void append(uint8_t* data, uint16_t size);

        const PsiDescriptor& getDescriptor();
};

class NitSection : public PsiSectionCommon
{
    public:
        struct TsInfo
        {
            uint16_t transportStreamId;
            uint16_t originalNetworkId;
            PsiDescriptor transportDescriptor;
        };
        typedef std::list<TsInfo> TsInfoList;

    private:
        uint8_t* start;
        PsiDescriptor networkDescriptor;
        TsInfoList tsInfoList;

        void onComplete();

    public:
        NitSection();
        ~NitSection();

        void parse(uint8_t* data, uint16_t size);
        void append(uint8_t* data, uint16_t size);

        uint16_t getNetworkId();
        const PsiDescriptor& getNetworkDescriptor();
        const TsInfoList& getTsInfoList();
};

// Use these macros only within this file and PsiTables.cpp

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

#define NIT_NDL_MASK                    0x0FFF
#define NIT_NDL_SHIFT                   8
#define NIT_TSLL_MASK                   0x0FFF
#define NIT_TSLL_SHIFT                  8
#define NIT_TSID_SHIFT                  8
#define NIT_ORIG_NWID_SHIFT             8
#define NIT_TSDL_MASK                   0x0FFF
#define NIT_TSDL_SHIFT                  8

#define NIT_GET_NW_DESCRIPTOR_LENGTH(x) (((x->byte0 << NIT_NDL_SHIFT) | (x->byte1)) & NIT_NDL_MASK)
#define NIT_GET_TS_LOOP_LENGTH(x)       (((x->byte0 << NIT_TSLL_SHIFT) | (x->byte1)) & NIT_TSLL_MASK)
#define NIT_GET_TSID(x)                 ((x->byte0 << NIT_TSID_SHIFT) | (x->byte1))
#define NIT_GET_ORIG_NWID(x)            ((x->byte2 << NIT_ORIG_NWID_SHIFT) | (x->byte3))
#define NIT_GET_TS_DESCRIPTOR_LENGTH(x) (((x->byte4 << NIT_TSDL_SHIFT) | (x->byte5)) & NIT_TSDL_MASK)


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
    this->PsiSectionCommon::parse(data, size, MpegConstants::TABLE_PAT);
}

inline void PatSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, MpegConstants::TABLE_PAT);
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
    this->PsiSectionCommon::parse(data, size, MpegConstants::TABLE_PMT);
}

inline void PmtSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, MpegConstants::TABLE_PMT);
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
    this->PsiSectionCommon::parse(data, size, MpegConstants::TABLE_CAT);
}

inline void CatSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, MpegConstants::TABLE_CAT);
}

inline const PsiDescriptor& CatSection::getDescriptor()
{
    return descriptor;
}

inline void TsdtSection::parse(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::parse(data, size, MpegConstants::TABLE_TSDT);
}

inline void TsdtSection::append(uint8_t* data, uint16_t size)
{
    this->PsiSectionCommon::append(data, size, MpegConstants::TABLE_TSDT);
}

inline const PsiDescriptor& TsdtSection::getDescriptor()
{
    return descriptor;
}

inline uint16_t NitSection::getNetworkId()
{
    return tableIdExtension;
}

inline const PsiDescriptor& NitSection::getNetworkDescriptor()
{
    return networkDescriptor;
}

inline const NitSection::TsInfoList& NitSection::getTsInfoList()
{
    return tsInfoList;
}

#endif
