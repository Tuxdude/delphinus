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

#include "PsiTables.h"
#include <cstddef>
#include <cassert>
#include <cstring>

#define GET_LESS(x, y) ((x) < (y) ? (x) : (y))

PsiSection::PsiSection()
    :   start(NULL),
        pointerField(0)
{
}

PsiSection::~PsiSection()
{
}

bool PsiSection::parse(uint8_t* data)
{
    // The first byte to parse should is the pointer field
    // giving the offset to where the section data starts
    pointerField = *data;
    start = data + pointerField + 1;
    return (PSI_GET_SSI(PSI_HEADER_START) == 1 &&
            PSI_GET_HARD_ZERO(PSI_HEADER_START) == 0 &&
            PSI_GET_TABLE_ID(PSI_HEADER_START) != 0xFF &&
            PSI_GET_LENGTH(PSI_HEADER_START) < 0x3FD);
}

PsiSectionCommon::PsiSectionCommon()
    :   isComplete(false),
        start(NULL),
        sectionLength(0),
        tableIdExtension(0),
        validSize(0),
        currentSection(0),
        lastSection(0)
        
{
}

PsiSectionCommon::~PsiSectionCommon()
{
    clear();
}

void PsiSectionCommon::clear()
{
    if (start)
    {
        delete[] start;
        start = NULL;
    }
    isComplete = false;
    sectionLength = 0;
    validSize = 0;
}

void PsiSectionCommon::parse(uint8_t* data, uint16_t size, uint8_t tableId)
{
    clear();
    // Dont include the pointer field
    data += *data + 1;
    // Subtract the number of bytes lost in the header - we dont store
    // the section header
    sectionLength = PSI_GET_LENGTH(((ByteField*)data)) - 5;
    tableIdExtension = PSI_GET_TABLE_ID_EXTN(((ByteField*)data));
    currentSection = PSI_GET_SECTION_NUMBER(((ByteField*)data));
    lastSection = PSI_GET_LAST_SECTION_NUMBER(((ByteField*)data));

    assert(PSI_GET_TABLE_ID(((ByteField*)data)) == tableId);
    assert(currentSection == 0);

    start = new uint8_t[sectionLength];
    // Copy the minimum of the available data size and the section length,
    // since there is no use copying padding bytes 0xFF
    uint16_t copyingSize = GET_LESS(size - 8, sectionLength);
    memcpy(start, data + 8, copyingSize);
    validSize = copyingSize;

    if (lastSection == currentSection)
    {
        assert(sectionLength == validSize);
        isComplete = true;
        onComplete();
    }
}

void PsiSectionCommon::append(uint8_t* data, uint16_t size, uint8_t tableId)
{
    // Verify that the section is already not complete
    assert(!isComplete && currentSection < lastSection);

    lastSection = PSI_GET_LAST_SECTION_NUMBER(((ByteField*)data));
    assert(PSI_GET_TABLE_ID(((ByteField*)data)) == tableId);
    assert(PSI_GET_SECTION_NUMBER(((ByteField*)data)) == currentSection + 1);
    assert(PSI_GET_LAST_SECTION_NUMBER(((ByteField*)data)) == lastSection);
    ++currentSection;

    // Copy the minimum of the available data size and the section length,
    // since there is no use copying padding bytes 0xFF
    uint16_t copyingSize = GET_LESS(size - 8, sectionLength - validSize);
    memcpy(start + validSize, data + 8, copyingSize);
    validSize += copyingSize;
    if (lastSection == currentSection)
    {
        assert(sectionLength == validSize);
        isComplete = true;
        onComplete();
    }
}

void PatSection::onComplete()
{
    // Parsing the program info from the section
    uint8_t* data = start;
    uint16_t remainingData = sectionLength;
    programList.clear();
    ProgramInfo info;
    while (remainingData > 4)
    {
        info.programNumber = PAT_GET_PROG_NUMBER(((ByteField*)data));
        info.pmtPid = PAT_GET_PID(((ByteField*)data));
        programList.push_back(info);
        data += 4;
        remainingData -= 4;
    }
}

PatSection::PatSection()
    :   networkPid(0x1FFF)
{
}

PatSection::~PatSection()
{
}

void PmtSection::onComplete()
{
    // Parsing the PMT info
    pcrPid = PMT_GET_PCR_PID(((ByteField*)start));
    
    programInfoDescriptor.size = PMT_GET_PROG_INFO_LENGTH(((ByteField*)start));
    programInfoDescriptor.start = start + 4;

    // Parse the Stream info
    uint8_t* data = start + 4 + programInfoDescriptor.size;
    uint16_t remainingData = sectionLength - 4 - programInfoDescriptor.size;
    streamList.clear();
    StreamInfo info;
    while (remainingData > 4)
    {
        info.streamType = PMT_GET_STREAM_TYPE(((ByteField*)data));
        info.pid = PMT_GET_ES_PID(((ByteField*)data));
        info.descriptor.size = PMT_GET_ES_INFO_LENGTH(((ByteField*)data));
        info.descriptor.start = data + 5;
        streamList.push_back(info);
        data += (5 + info.descriptor.size);
        remainingData -= (5 + info.descriptor.size);
    }
}

PmtSection::PmtSection()
    :   pcrPid(0x1FFF)
{
    programInfoDescriptor.start = NULL;
    programInfoDescriptor.size = 0;
}

PmtSection::~PmtSection()
{
}

const char* PmtSection::getStreamTypeStr(uint8_t streamType)
{
    switch(streamType)
    {
        case STREAM_TYPE_RESERVED:
            return "ITU-T | ISO/IEC Reserved";
        case STREAM_TYPE_11172_VIDEO:
            return "MPEG-1 Video";
        case STREAM_TYPE_13818_2_VIDEO:
            return "MPEG-2 Video";
        case STREAM_TYPE_11172_AUDIO:
            return "MPEG-1 Audio";
        case STREAM_TYPE_13818_3_AUDIO:
            return "MPEG-2 Audio";
        case STREAM_TYPE_13818_1_PRIVATE_SECTIONS:
            return "ISO 13818-1 private_sections";
        case STREAM_TYPE_13818_1_PES_PRIVATE_DATA:
            return "ISO 13818-1 PES packets containing private data";
        case STREAM_TYPE_13522_MHEG:
            return "ISO 13522 MHEG";
        case STREAM_TYPE_13818_1_DSMCC:
            return "ISO/IEC 13818-1 DSM-CC";
        case STREAM_TYPE_H222_1:
            return "ITU-T Rec. H.222.1";
        case STREAM_TYPE_13818_6_TYPE_A:
            return "ISO 13818-6 type A";
        case STREAM_TYPE_13818_6_TYPE_B:
            return "ISO 13818-6 type B";
        case STREAM_TYPE_13818_6_TYPE_C:
            return "ISO 13818-6 type C";
        case STREAM_TYPE_13818_6_TYPE_D:
            return "ISO 13818-6 type D";
        case STREAM_TYPE_13818_1_AUX:
            return "ISO 13818-1 auxiliary";
        case STREAM_TYPE_13818_7_AAC_ADTS:
            return "AAC Audio with ADTS";
        case STREAM_TYPE_14496_2_VIDEO:
            return "MPEG-4 Part 2 Video";
        case STREAM_TYPE_14496_3_AAC_LATM:
            return "AAC Audio with LATM";
        case STREAM_TYPE_14496_1_FLEXMUX_PES:
            return "ISO 14496-1 SL-packetized stream or FlexMux stream carried in PES packets";
        case STREAM_TYPE_14496_1_FLEXMUX_SECTIONS:
            return "ISO 14496-1 SL-packetized stream or FlexMux stream carried in ISO 14496 sections.";
        case STREAM_TYPE_13818_6_SYNC_DOWNLOAD_PROTOCOL:
            return "ISO/IEC 13818-6 Synchronized Download Protocol";
        case STREAM_TYPE_14496_10_VIDEO:
            return "H.264 Video";
        case STREAM_TYPE_AVS_VIDEO:
            return "AVS Video";
        case STREAM_TYPE_DC_II_VIDEO:
            return "DigiCipher II Video / LPCM Audio";
        case STREAM_TYPE_A52_AC3_AUDIO:
            return "A52/AC-3 Audio";
        case STREAM_TYPE_HDMV_DTS_AUDIO:
            return "HDMV DTS Audio / DVD SPU subtitles";
        case STREAM_TYPE_LPCM_AUDIO:
            return "LPCM Audio / BD TrueHD Audio";
        case STREAM_TYPE_SDDS_AUDIO:
            return "SDDS Audio / E-AC-3 Audio";
        case STREAM_TYPE_ATSC_PROGRAM_ID:
            return "ATSC Program ID / DTS-HD HR Audio";
        case STREAM_TYPE_DTS_HD_MASTER_AUDIO:
            return "DTS-HD Master Audio";
        case STREAM_TYPE_E_AC3_AUDIO:
            return "E-AC-3 Audio";
        case STREAM_TYPE_DTS_AUDIO:
            return "DTS Audio";
        case STREAM_TYPE_A52B_AC3_AUDIO:
            return "A52b/AC-3 Audio / BD Interactive Graphics Stream";
        case STREAM_TYPE_DVD_SPU_VLS_SUBTITLE:
            return "SVD_SPU vls Subtitle";
        case STREAM_TYPE_SDDS_AUDIO_2:
            return "SDDS Audio";
        case STREAM_TYPE_MSCODEC_VIDEO:
            return "MSCODEC Video";
        case STREAM_TYPE_VC_1_AP:
            return "VC-1 Advanced profile Video";
        case STREAM_TYPE_VC_1_SMP:
            return "VC-1 Simple/Main Profile Video";
        default:
            {
                if (streamType >= STREAM_TYPE_USER_PRIVATE_START)
                {
                    return "User Private";
                }
                else if (streamType >= STREAM_TYPE_13818_1_RESERVED_START &&
                         streamType <= STREAM_TYPE_13818_1_RESERVED_END)
                {
                    return "ISO 13818-1 Reserved";
                }
            }
    }
    return "Unknown";
}

CatSection::CatSection()
{
    descriptor.start = NULL;
    descriptor.size = 0;
}

CatSection::~CatSection()
{
}

void CatSection::onComplete()
{
    // Store the CA descriptor details
    descriptor.start = start;
    // Subtract the size of the CRC_32
    descriptor.size = sectionLength -4;
}

