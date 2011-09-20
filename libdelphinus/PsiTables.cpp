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
    pointerField = *data + 1;
    start = data + pointerField;
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
    sectionLength = PSI_GET_LENGTH(((PsiSection::PsiSectionHeader*)data)) - 5;
    tableIdExtension = PSI_GET_TABLE_ID_EXTN(((PsiSection::PsiSectionHeader*)data));
    currentSection = PSI_GET_SECTION_NUMBER(((PsiSection::PsiSectionHeader*)data));
    lastSection = PSI_GET_LAST_SECTION_NUMBER(((PsiSection::PsiSectionHeader*)data));

    assert(PSI_GET_TABLE_ID(((PsiSection::PsiSectionHeader*)data)) == tableId);
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

    lastSection = PSI_GET_LAST_SECTION_NUMBER(((PsiSection::PsiSectionHeader*)data));
    assert(PSI_GET_TABLE_ID(((PsiSection::PsiSectionHeader*)data)) == tableId);
    assert(PSI_GET_SECTION_NUMBER(((PsiSection::PsiSectionHeader*)data)) == currentSection + 1);
    assert(PSI_GET_LAST_SECTION_NUMBER(((PsiSection::PsiSectionHeader*)data)) == lastSection);
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
        info.programNumber = PAT_GET_PROG_NUMBER(((PatSection::PatProgramInfo*)data));
        info.pmtPid = PAT_GET_PID(((PatSection::PatProgramInfo*)data));
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
    // TODO:
    // Parsing the PMT info
}

PmtSection::PmtSection()
    :   pcrPid(0x1FFF),
        programInfoDescriptor(NULL)
{
}

PmtSection::~PmtSection()
{
}

CatSection::CatSection()
{
}

CatSection::~CatSection()
{
}

void CatSection::onComplete()
{
    // TODO:
    // Parsing the CAT info
}

