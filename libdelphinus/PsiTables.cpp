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

PsiSection::PsiSection()
    : start(NULL)
{
}

PsiSection::~PsiSection()
{
}

inline bool PsiSection::parse(uint8_t* data)
{
    // The first byte to parse should is the pointer field
    // giving the offset to where the section data starts
    start = data + *(data);
    return (PSI_GET_SSI(PSI_HEADER_START) == 1 &&
            PSI_GET_HARD_ZERO(PSI_HEADER_START) == 0);
}

PatSection::PatSection()
    : start(NULL)
{
}

PatSection::~PatSection()
{
}

void PatSection::parse(uint8_t* data)
{
    start = data;
    assert(PSI_GET_TABLE_ID(PSI_HEADER_START) == PsiSection::TABLE_PAT);
    transportStreamId = PSI_GET_TABLE_ID_EXTN(PSI_HEADER_START);

    uint16_t sectionLength = PSI_GET_LENGTH(PSI_HEADER_START);
    // We do not yet support sections split across TS packets
    assert(PSI_GET_SECTION_NUMBER(PSI_HEADER_START) == 0);
}

