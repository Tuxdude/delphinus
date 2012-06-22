/*
 *  MpegConstants.h - declaration of the constants found in the MPEG, DVB,
 *  ATSC, DCII, SCTE and other related standards.
 *
 *  This file is part of delphinus - a stream analyzer for various multimedia
 *  stream formats and containers.
 *
 *  Copyright (C) 2012 Ash (Tuxdude)
 *  tuxdude.github@gmail.com
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
 *  \file   MpegConstants.h
 *  \brief  Constants related to MPEG2, DVB, ATSC, SCTE and related standards.
 *
 *  This file defines all the standard constants available in the various
 *  digital television standards such as MPEG-2, DVB, ATSC, SCTE and related
 *  ones.
 */

#ifndef DELPHINUS_MPEG_CONSTANTS_H
#define DELPHINUS_MPEG_CONSTANTS_H
#include "common/DelphinusUtils.h"

/**
 *  \brief  The namespace in which all the MPEG related constants are defined.
 *
 *  All the constants in the file MpegConstants.h are defined under the
 *  MpegConstants namespace.
 */
namespace MpegConstants
{
/**
 *  \brief  Constants for TS Packet sizes.
 */
    enum TsPacketSize
    {
        /** 188 bytes TS Packet */
        PACKET_SIZE_TS              = 188,
        /** 192 bytes TTS Packet */
        PACKET_SIZE_TTS             = 192
    };

/**
 *  \public
 *  \brief  Constants for standard PID values.
 */
    enum Pid
    {
/** PAT PID */
        PID_PAT                     = 0x0000,
/** CAT PID */
        PID_CAT                     = 0x0001,
/** TSDT PID */
        PID_TSDT                    = 0x0002,
/** NIT PID */
        PID_NIT                     = 0x0010,
/** BAT PID */
        PID_SDT_BAT                 = 0x0011,
/** EIT/CIT PID */
        PID_EIT_CIT                 = 0x0012,
/** RST PID */
        PID_RST                     = 0x0013,
/** TDT/TOT PID */
        PID_TDT_TOT                 = 0x0014,
/** Network Synchronization PID */
        PID_NETWORK_SYNC            = 0x0015,
/** RNT PID */
        PID_RNT                     = 0x0016,
/** Inband signalling PID */
        PID_INBAND_SIGNALLING       = 0x001C,
/** Measurement PID */
        PID_MEASUREMENT             = 0x001D,
/** DIT PID */
        PID_DIT                     = 0x001E,
/** SIT PID */
        PID_SIT                     = 0x001F,
/** NULL PID */
        PID_NULL                    = 0x1FFF
    };

/**
 *  \brief  Constants for Stream IDs in the PES header.
 */
    enum PesStreamId
    {
        STREAM_ID_PROGRAM_STREAM_MAP    = 0xBC,
        STREAM_ID_PRIVATE_1             = 0xBD,
        STREAM_ID_PADDING               = 0xBE,
        STREAM_ID_PRIVATE_2             = 0xBF,
        STREAM_ID_AUDIO_START           = 0xC0,
        STREAM_ID_AUDIO_END             = 0xDF,
        STREAM_ID_VIDEO_START           = 0xE0,
        STREAM_ID_VIDEO_END             = 0xEF,
        STREAM_ID_ECM                   = 0xF0,
        STREAM_ID_EMM                   = 0xF1,
        //TODO: find a better name
        STREAM_ID_DSMCC_STREAM          = 0xF2,
        STREAM_ID_ISO_IEC_13522_STREAM  = 0xF3,
        STREAM_ID_H222_1_TYPE_A         = 0xF4,
        STREAM_ID_H222_1_TYPE_B         = 0xF5,
        STREAM_ID_H222_1_TYPE_C         = 0xF6,
        STREAM_ID_H222_1_TYPE_D         = 0xF7,
        STREAM_ID_H222_1_TYPE_E         = 0xF8,
        STREAM_ID_ANCILLARY             = 0xF9,
        STREAM_ID_SL_PACKETIZED         = 0xFA,
        STREAM_ID_FLEXMUX               = 0xFB,
        STREAM_ID_PROGRAM_STREAM_DIR    = 0xFF
    };

/**
 *  \brief  Constants for Stream Type values in the PMT.
 */
    enum PmtStreamType
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
        // Auxillary
        STREAM_TYPE_H222_1                          = 0x09,
        // Multi protocol encapsulation
        STREAM_TYPE_13818_6_TYPE_A                  = 0x0A,
        // DSM-CC U-N messages
        STREAM_TYPE_13818_6_TYPE_B                  = 0x0B,
        // Stream descriptors
        STREAM_TYPE_13818_6_TYPE_C                  = 0x0C,
        // Sections
        STREAM_TYPE_13818_6_TYPE_D                  = 0x0D,
        STREAM_TYPE_13818_1_AUX                     = 0x0E,
        STREAM_TYPE_13818_7_AAC_ADTS                = 0x0F,
        STREAM_TYPE_14496_2_VIDEO                   = 0x10,
        STREAM_TYPE_14496_3_AAC_LATM                = 0x11,
        STREAM_TYPE_14496_1_FLEXMUX_PES             = 0x12,
        STREAM_TYPE_14496_1_FLEXMUX_SECTIONS        = 0x13,
        STREAM_TYPE_13818_6_SYNC_DOWNLOAD_PROTOCOL  = 0x14,
        STREAM_TYPE_14496_10_VIDEO                  = 0x1b,
        // Chinese standard
        STREAM_TYPE_AVS_VIDEO                       = 0x42,
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

/**
 *  \brief  Constants for Table ID values in the section header.
 */
    enum PsiTableId
    {
        TABLE_PAT                           = 0x00,
        TABLE_CAT                           = 0x01,
        TABLE_PMT                           = 0x02,
        TABLE_TSDT                          = 0x03,
        TABLE_SCENE_DESCRIPTION             = 0x04,
        TABLE_OBJECT_DESCRIPTION            = 0x05,
        TABLE_13818_1_RESERVED_START        = 0x06,
        TABLE_13818_1_RESERVED_END          = 0x37,
        TABLE_13818_6_START                 = 0x38,
        TABLE_13818_6_END                   = 0x3F,
        TABLE_NETWORK_INFO_ACTUAL           = 0x40,
        TABLE_NETWORK_INFO_OTHER            = 0x41,
        TABLE_SERVICE_DESCRIPTION_ACTUAL    = 0x42,
        TABLE_SERVICE_DESCRIPTION_OTHER     = 0x46,
        TABLE_BAT                           = 0x4A,
        TABLE_EIT_ACTUAL_PRESENT_FOLLOWING  = 0x4E,
        TABLE_EIT_OTHER_PRESENT_FOLLOWING   = 0x4F,
        TABLE_EIT_ACTUAL_SCHEDULE_START     = 0x50,
        TABLE_EIT_ACTUAL_SCHEDULE_END       = 0x5F,
        TABLE_EIT_OTHER_SCHEDULE_START      = 0x60,
        TABLE_EIT_OTHER_SCHEDULE_END        = 0x6F,
        TABLE_TDT                           = 0x70,
        TABLE_RST                           = 0x71,
        TABLE_STUFFING                      = 0x72,
        TABLE_TOT                           = 0x73,
        TABLE_APP_INFO                      = 0x74,
        TABLE_CONTAINER                     = 0x75,
        TABLE_RELATED_CONTENT               = 0x76,
        TABLE_CONTENT                       = 0x77,
        TABLE_MPE_FEC                       = 0x78,
        TABLE_RESOLUTION_NOTIFICATION       = 0x79,
        TABLE_MPE_IFEC                      = 0x7A,
        TABLE_DISCONTINUITY_INFO            = 0x7E,
        TABLE_SELECTION_INFO                = 0x7F,
        TABLE_DVB_USER_DEFINED_START        = 0x80,
        TABLE_DVB_USER_DEFINED_END          = 0xFE,
        TABLE_13818_1_USER_PRIVATE_START    = 0x40,
        TABLE_13818_1_USER_PRIVATE_END      = 0xFE,
        TABLE_FORBIDDEN                     = 0xFF
    };

/**
 *  \brief  Constants for Tags used in the Descriptors.
 */
    enum DescriptorTag
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
}


#endif
