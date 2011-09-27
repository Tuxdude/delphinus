/*
 *  Descriptors.h - declaration of MPEG, DVB, ATSC, DCII descriptors
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

#ifndef DELPHINUS_DESCRIPTORS_H
#define DELPHINUS_DESCRIPTORS_H
#include "common/DelphinusUtils.h"

namespace Descriptors
{
    enum
    {
        // From ISO 13818-1
        TAG_VIDEO_STREAM                    = 0x02,
        TAG_AUDIO_STREAM                    = 0x03,
        TAG_HIRERACHY                       = 0x04,
        TAG_REGISTRATION                    = 0x05,
        TAG_DATA_STREAM_ALIGNMENT           = 0x06,
        TAG_TARGET_BACKGROUND_GRID          = 0x07,
        TAG_VIDEO_WINDOW                    = 0x08,
        TAG_CA                              = 0x09,
        TAG_ISO_639_LANGUAGE                = 0x0A,
        TAG_SYSTEM_CLOCK                    = 0x0B,
        TAG_MULTIPLEX_BUFFER_UTILIZATION    = 0x0C,
        TAG_COPYRIGHT                       = 0x0D,
        TAG_MAXIMUM_BITRATE                 = 0x0E,
        TAG_PRIVATE_DATA_INDICATOR          = 0x0F,
        TAG_SMOOTHING_BUFFER                = 0x10,
        TAG_STD                             = 0x11,
        TAG_IBP                             = 0x12,
        // FIXME - fill in from 13818-6
        TAG_13818_6_START                   = 0x13,
        TAG_13818_6_END                     = 0x1A,
        TAG_MPEG_4_VIDEO                    = 0x1B,
        TAG_MPEG_4_AUDIO                    = 0x1C,
        TAG_IOD                             = 0x1D,
        TAG_SL                              = 0x1E,
        TAG_FMC                             = 0x1F,
        TAG_EXTERNAL_ES_ID                  = 0x20,
        TAG_MUXCODE                         = 0x21,
        TAG_FMX_BUFFER_SIZE                 = 0x22,
        TAG_MULTIPLEX_BUFFER                = 0x23,
        TAG_13818_1_RESERVED_START          = 0x24,
        TAG_13818_1_RESERVED_END            = 0x3F,
        TAG_13818_1_USER_PRIVATE_START      = 0x40,
        TAG_13818_1_USER_PRIVATE_END        = 0xFF,

        // From DVB
        TAG_NETWORK_NAME                    = 0x40,
        TAG_SERVICE_LIST                    = 0x41,
        TAG_STUFFING                        = 0x42,
        TAG_SATELLITE_DELIVERY              = 0x43,
        TAG_CABLE_DELIVERY                  = 0x44,
        TAG_VBI_DATA                        = 0x45,
        TAG_VBI_TELETEXT                    = 0x46,
        TAG_BOUQUET                         = 0x47,
        TAG_SERVICE                         = 0x48,
        TAG_COUNTRY_AVAILABILITY            = 0x49,
        TAG_LINKAGE                         = 0x4A,
        TAG_NVOD                            = 0x4B,
        TAG_TIME_SHIFTED                    = 0x4C,
        TAG_COMPONENT                       = 0x50,
        TAG_MOSAIC                          = 0x51,
        TAG_STREAM_IDENTIFIER               = 0x52,
        TAG_CA_IDENTIFIER                   = 0x53,
        TAG_CONTENT                         = 0x54,
        TAG_PARENTAL_RATING                 = 0x55,
        TAG_TELETEXT                        = 0x56,
        TAG_TELEPHONE                       = 0x57,
        TAG_LOCAL_TIME_OFFSET               = 0x58,
        TAG_SUBTITLING                      = 0x59,
        TAG_TERRESTRIAL_DELIVERY_SYSTEM     = 0x5A,
        TAG_MULTILINGUAL_NEWTORK_NAME       = 0x5B,
        TAG_MULTILINGUAL_BOUQUET_NAME       = 0x5C,
        TAG_MULTILINGUAL_SERVICE_NAME       = 0x5D,
        TAG_MULTILINGUAL_COMPONENT          = 0x5E,
        TAG_PRIVATA_DATA_SPECIFIER          = 0x5F,
        TAG_SERVICE_MOVE                    = 0x60,
        TAG_SHORT_SMOOTHING_BUFFER          = 0x61,
        TAG_FREQUENCY_LIST                  = 0x62,
        TAG_PARTIAL_TS                      = 0x63,
        TAG_DATA_BROADCAST                  = 0x64,
        TAG_SCRAMBLING                      = 0x65,
        TAG_DATA_BROADCAST_ID               = 0x66,
        TAG_TS                              = 0x67,
        TAG_DSNG                            = 0x68,
        TAG_PDC                             = 0x69,
        TAG_AC3                             = 0x6A,
        TAG_ANCILLARY_DATA                  = 0x6B,
        TAG_CELL_LIST                       = 0x6C,
        TAG_CELL_FREQUENCY_LIST             = 0x6D,
        TAG_ANNOUNCEMENT_SUPPORT            = 0x6E,
        TAG_APPLICATION_SIGNALLING          = 0x6F,
        TAG_APPLICATION_FIELD               = 0x70,
        TAG_SERVICE_IDENTIFIER              = 0x71,
        TAG_SERVICE_AVAILABILITY            = 0x72,
        TAG_DEFAULT_AUTHORITY               = 0x73,
        TAG_RELATED_CONTENT                 = 0x74,
        TAG_TVA_ID                          = 0x75,
        TAG_CONTENT_ID                      = 0x76,
        TAG_TIME_SLICE_FEC_IDENTIFIER       = 0x77,
        TAG_ECM_REPETITION_RATE             = 0x78,
        TAG_S2_SATELLITE_DELIVERY_SYSTEM    = 0x79,
        TAG_E_AC3                           = 0x7A,
        TAG_DTS                             = 0x7B,
        TAG_AAC                             = 0x7C,
        TAG_XAIT_LOCATION                   = 0x7D,
        TAG_FTA_CONTENT_MANAGEMENT          = 0x7E,
        TAG_EXTENSION                       = 0x7F,
        TAG_DVB_USER_DEFINED_START          = 0x80,
        TAG_DVB_USER_DEFINED_END            = 0xFE,
        TAG_FORBIDDEN                       = 0xFF
    };
};


#endif
