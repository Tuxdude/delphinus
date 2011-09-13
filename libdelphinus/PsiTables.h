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

// Table ID                     8
// section syntax indicator     1
// 0                            1
// reserved                     2
// section length               12
// Intermediate field           16
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

    public:
        PsiSection();
        ~PsiSection();

        //Note: the first byte to parse should be the pointer field
        //i.e. the first byte of the payload
        bool parse(uint8_t* data);
        uint8_t getTableId();
        uint16_t getLength();
        uint16_t getIntermediateField();
        uint8_t getVersionNumber();
        bool getCurrentNextIndicator();
        uint8_t getSectionNumber();
        uint8_t getLastSectionNumber();
        uint8_t* getData();
};

#define PSI_SSI_SHIFT                   7
#define PSI_HARD_ZERO_MASK              0x40
#define PSI_HARD_ZERO_SHIFT             6
#define PSI_LENGTH_MASK                 0x0F
#define PSI_LENGTH_SHIFT                8
#define PSI_IF_SHIFT                    8
#define PSI_VERSION_MASK                0x3E
#define PSI_VERSION_SHIFT               1
#define PSI_CURR_NEXT_MASK              0x01

#define PSI_GET_TABLE_ID(x)             (x->byte0)
#define PSI_GET_SSI(x)                  (x->byte1 >> PSI_SSI_SHIFT)
#define PSI_GET_HARD_ZERO(x)            ((x->byte1 & PSI_HARD_ZERO_MASK) >> PSI_HARD_ZERO_SHIFT)
#define PSI_GET_LENGTH(x)               (((x->byte1 & PSI_LENGTH_MASK) << PSI_LENGTH_SHIFT) |\
                                         (x->byte2))
#define PSI_GET_IF(x)                   (((x->byte3) << PSI_IF_SHIFT) | (x->byte4))
#define PSI_GET_VERSION(x)              ((x->byte5 & PSI_VERSION_MASK) >> PSI_VERSION_SHIFT)
#define PSI_GET_CURR_NEXT(x)            ((x->byte5) & PSI_CURR_NEXT_MASK)
#define PSI_GET_SECTION_NUMBER(x)       (x->byte6)
#define PSI_GET_LAST_SECTION_NUMBER(x)  (x->byte7)

#define PSI_HEADER_START                ((PsiSectionHeader*)(start))

inline uint8_t PsiSection::getTableId()
{
    return PSI_GET_TABLE_ID(PSI_HEADER_START);
}

inline uint16_t PsiSection::getLength()
{
    return PSI_GET_LENGTH(PSI_HEADER_START);
}

inline uint16_t PsiSection::getIntermediateField()
{
    return PSI_GET_IF(PSI_HEADER_START);
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

#endif
