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

/**
 *  \brief  PsiDescriptor represents a descriptor.
 */
struct PsiDescriptor
{
/**  Start address of the descriptor. */
    uint8_t* start;
/**  Size of the descriptor. */
    uint16_t size;
};

/**
 *  \brief  PsiSection represents a standard section.
 *
 *  PsiSection is used to parse a standard section, and get the various
 *  fields in the section header.
 */
class PsiSection
{
    private:
        uint8_t* start;
        uint8_t pointerField;

    public:
        PsiSection();
        ~PsiSection();

/**
 *  \brief  Parse the given data as a section.
 *  \param  data The start of the data to be parsed.
 *          \note The first byte in data should be the pointer field, i.e.
 *          the very first byte in the TsPacket payload.
 *  \return Is a valid section or not.
 */
        bool parse(uint8_t* data);
/**
 *  \brief  Get the value of the pointer field in the TS payload.
 *  \return Value of the Pointer field.
 */
        uint8_t getPointerField();
/**
 *  \brief  Get the offset of the actual section data from the start of the
 *          payload.
 *  \return Offset in bytes from the start of the payload.
 */
        uint8_t getDataOffset();
/**
 *  \brief  Get the Table ID field in the section header.
 *  \return 8-bit Table ID field in the section header.
 */
        uint8_t getTableId();
/**
 *  \brief  Get the length field in the section header.
 *  \return 16-bit length field in the section header.
 */
        uint16_t getLength();
/**
 *  \brief  Get the Table ID Extension field in the section header.
 *  \return 16-bit Table ID Extension field in the section header.
 */
        uint16_t getTableIdExtension();
/**
 *  \brief  Get the version number field in the section header.
 *  \return 5-bit version number field in the section header.
 */
        uint8_t getVersionNumber();
/**
 *  \brief  Get the Current Next Indicator flag in the section header.
 *  \return Current Next Indicator flag in the section header..
 */
        bool getCurrentNextIndicator();
/**
 *  \brief  Get the Section Number field in the section header.
 *  \return 8-bit Section Number field in the section header.
 */
        uint8_t getSectionNumber();
/**
 *  \brief  Get the Last Section Number field in the section header.
 *  \return 8-bit Last Section Number field in the section header.
 */
        uint8_t getLastSectionNumber();
/**
 *  \brief  Get the data bytes in the section.
 *  \return Address of the start of the data.
 */
        uint8_t* getData();

        friend class PsiSectionCommon;
};

/**
 *  \brief  Base class of all the standard sections.
 *
 *  PsiSectionCommon is the base class of all the standard sections and
 *  provides helper methods to parse and validate sections easily.
 */
class PsiSectionCommon
{
    protected:
/** The entire Section is complete or not. */
        bool isComplete;
/** Start of the section data. */
        uint8_t* start;
/** Length of the section excluding the section header bytes. */
        uint16_t sectionLength;
/** Table ID Extension */
        uint16_t tableIdExtension;
/** Size of the data currently in the buffer. */
        uint16_t validSize;
/** Current Section Number */
        uint8_t currentSection;
/** Section number of the last section to make the entire section complete. */
        uint8_t lastSection;

        PsiSectionCommon();
        virtual ~PsiSectionCommon();
/**
 *  \brief  The function to be called when parsing the section is successfull.
 */
        virtual void onComplete() = 0;
/**
 *  \brief  Parse the given data as the start of a section with the given
 *          tableId.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 *  \param  tableId The Table ID to be checked against in the section header.
 */
        void parse(uint8_t* data, uint16_t size, uint8_t tableId);
/**
 *  \brief  Parse and append the given data as the subsequent section with
 *          the given tableId.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 *  \param  tableId The Table ID to be checked against in the section header.
 */
        void append(uint8_t* data, uint16_t size, uint8_t tableId);

    public:
/**
 *  \brief  Clear the section handle.
 */
        void clear();
/**
 *  \brief  Check if the section is complete.
 *  \return True if the section is complete, false otherwise.
 */
        bool isCompleteSection();
};

/**
 *  \brief  Represents a PAT section.
 *
 *  PatSection represents a PAT section and is used for parsing the PAT.
 */
class PatSection : public PsiSectionCommon
{
    public:
/**
 *  \brief  Represents information about a single program.
 */
        struct ProgramInfo
        {
/** Program Number */
            uint16_t programNumber;
/** PMT PID */
            uint16_t pmtPid;
        };
/**
 *  \brief  List of all programs, typically to represent the information
 *          extracted from the PAT.
 */
        typedef std::list<ProgramInfo> ProgramList;

    private:
        ProgramList programList;
        uint16_t networkPid;

        void onComplete();

    public:
        PatSection();
        ~PatSection();

/**
 *  \brief  Parse the given data as the start of a PAT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void parse(uint8_t* data, uint16_t size);
/**
 *  \brief  Parse and append the given data as the subsequent PAT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void append(uint8_t* data, uint16_t size);
/**
 *  \brief  Get the Transport Stream ID from the PAT.
 *  \return Transport Stream ID from the PAT.
 */
        uint16_t getTransportStreamId();
/**
 *  \brief  Get the list of programs in the PAT.
 *  \return List of programs in the PAT.
 */
        const ProgramList& getPrograms();
/**
 *  \brief  Get the Newtork PID from the PAT
 *  \return Network PID in the PAT if one exists, MpegConstants::PID_NULL
 *          otherwise.
 */
        uint16_t getNetworkPid();
};

/**
 *  \brief  Represents a CAT section.
 *
 *  CatSection represents a CAT section and is used for parsing the CAT.
 */
class CatSection : public PsiSectionCommon
{
    private:
        uint8_t* start;
        PsiDescriptor descriptor;

        void onComplete();

    public:
        CatSection();
        ~CatSection();
/**
 *  \brief  Parse the given data as the start of a CAT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void parse(uint8_t* data, uint16_t size);
/**
 *  \brief  Parse and append the given data as the subsequent CAT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void append(uint8_t* data, uint16_t size);
/**
 * \brief   Get the CA descriptor in the CAT section.
 * \return  CA descriptor in the CAT section.
 */
        const PsiDescriptor& getDescriptor();
};

/**
 *  \brief  Represents a PMT section.
 *
 *  PmtSection represents a PMT section and is used for parsing the PMT.
 */
class PmtSection : public PsiSectionCommon
{
    public:
/**
 *  \brief  Represents the information about a single stream in the PMT's
 *          stream list.
 */
        struct StreamInfo
        {
/** 8-bit Stream Type */
            uint8_t streamType;
/** 13-bit PID */
            uint16_t pid;
/** Stream descriptor */
            PsiDescriptor descriptor;
        };
/**
 *  \brief  Represents the information about a set of streams typically to
 *          represent the entire PMT's stream list.
 */
        typedef std::list<StreamInfo> StreamList;

    private:
        uint16_t pcrPid;
        PsiDescriptor programInfoDescriptor;
        StreamList streamList;

        void onComplete();

    public:
        PmtSection();
        ~PmtSection();

/**
 *  \brief  Get a string description for a given stream type.
 *  \param  streamType 8-bit stream type value from the PMT.
 *  \return Descriptive string of the stream type.
 */
        static const char* getStreamTypeStr(uint8_t streamType);
/**
 *  \brief  Parse the given data as the start of a PMT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void parse(uint8_t* data, uint16_t size);
/**
 *  \brief  Parse and append the given data as the subsequent PMT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void append(uint8_t* data, uint16_t size);

/**
 *  \brief  Get the Program number corresponding to the PMT
 *  \return 16-bit Program number.
 */
        uint16_t getProgramNumber();
/**
 *  \brief  Get the PCR PID of the program from the PMT.
 *  \return 13-bit PCR PID of the program.
 */
        uint16_t getPcrPid();
/**
 *  \brief  Get the Program Information descriptor in the PMT.
 *  \return Program Information Descriptor in the PMT.
 */
        const PsiDescriptor& getProgramInfoDescriptor();
/**
 *  \brief  Get the list of all streams in the PMT.
 *  \return Information about each of the streams in the PMT.
 */
        const StreamList& getStreamList();
};

/**
 *  \brief  Represents a TSDT section.
 *
 *  TsdtSection represents a TSDT section and is used for parsing the TSDT.
 */
class TsdtSection : public PsiSectionCommon
{
    private:
        uint8_t* start;
        PsiDescriptor descriptor;

        void onComplete();

    public:
        TsdtSection();
        ~TsdtSection();
/**
 *  \brief  Parse the given data as the start of a TSDT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void parse(uint8_t* data, uint16_t size);
/**
 *  \brief  Parse and append the given data as the subsequent TSDT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void append(uint8_t* data, uint16_t size);
/**
 *  \brief  Get the TS descriptor in the TSDT section.
 *  \return TS descriptor in the TSDT section.
 */
        const PsiDescriptor& getDescriptor();
};

/**
 *  \brief  Represents a TSDT section.
 *
 *  TsdtSection represents a TSDT section and is used for parsing the TSDT.
 */
class NitSection : public PsiSectionCommon
{
    public:
/**
 *  \brief Individual TS info, typically gathered from the NIT section. 
 */
        struct TsInfo
        {
/** Transport Stream ID.  */
            uint16_t transportStreamId;
/** Original Network ID. */
            uint16_t originalNetworkId;
/** Transport Descriptor. */
            PsiDescriptor transportDescriptor;
        };
/**
 *  \brief  A list of information about multiple Transport Streams, typically
 *          gathered from the NIT section.
 */
        typedef std::list<TsInfo> TsInfoList;

    private:
        uint8_t* start;
        PsiDescriptor networkDescriptor;
        TsInfoList tsInfoList;

        void onComplete();

    public:
        NitSection();
        ~NitSection();
/**
 *  \brief  Parse the given data as the start of a PAT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void parse(uint8_t* data, uint16_t size);
/**
 *  \brief  Parse and append the given data as the subsequent PAT section.
 *  \param  data Start of the data to parse.
 *  \param  size The maximum size of the data that should be parsed.
 *          Useful when there are subsequent sections in which the
 *          data for the table continues in.
 */
        void append(uint8_t* data, uint16_t size);

/**
 *  \brief  Get the Network ID in the NIT section.
 *  \return 16-bit Network ID in the NIT section.
 */
        uint16_t getNetworkId();
/**
 *  \brief  Get the Network descriptor in the NIT section.
 *  \return Network descriptor in the NIT section.
 */
        const PsiDescriptor& getNetworkDescriptor();
/**
 *  \brief  Get the information about all the Transport Streams from the
 *          NIT section.
 *  \return Information about all the Transport streams from the NIT section.
 */
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

#define PSI_HEADER_START                ((DelphinusUtils::ByteField*)start)

#define PAT_PROG_NUMBER_SHIFT           8
#define PAT_PID_MASK                    0x1FFF
#define PAT_PID_SHIFT                   8

#define PAT_GET_PROG_NUMBER(x)          (((x->byte0) << PAT_PROG_NUMBER_SHIFT) | (x->byte1))
#define PAT_GET_PID(x)                  (((x->byte2 << PAT_PROG_NUMBER_SHIFT) | (x->byte3)) & PAT_PID_MASK)
#define PAT_PROG_INFO_START             ((DelphinusUtils::ByteField*)(start + offset))

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
