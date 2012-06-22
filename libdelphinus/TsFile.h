/*
 *  TsFile.h - An abstraction for handling the raw Transport Stream
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
 *  \file   TsFile.h
 *  \brief  A file abstraction to handle raw TS files.
 *
 *  Defines TsFile class for abstracting a Transport Stream (TS) file,
 *  accessing the file as individual TS packets, and various other helper
 *  methods.
 */

#ifndef DELPHINUS_TSFILE_H
#define DELPHINUS_TSFILE_H
#include <cstdio>
#include "Ts.h"
#include "Pes.h"
#include "PsiTables.h"

/**
 *  \brief  A file abstraction to handle raw TS files.
 *
 *  TsFile provides an abstraction for a Transport Stream (TS) file and
 *  provides the ability to access the TS file as individual TS packets. It
 *  also parses the PAT, PMT and other standard tables present in the TS and
 *  populates the metadata on opening the file.
 */
class TsFile
{
    public:
/**
 *  \brief  PAT information.
 */
        struct PatInfo
        {
/** Packet number(starts at 0) in the TS file where the PAT was located. */
            uint64_t packetNumber;
/** Transport Stream ID mentioned in the PAT. */
            uint16_t transportStreamId;
/** List of all the programs mentioned in the PAT. */
            PatSection::ProgramList programList;
        };
/**
 *  \brief  PMT information.
 */
        struct PmtInfo
        {
/** Packet number(starts at 0) in the TS file where the PMT was located. */
            uint64_t packetNumber;
/** PMT PID. */
            uint16_t pmtPid;
/** Program number mentioned in the PMT. */
            uint16_t programNumber;
/** PCR PID of the program mentioned in the PMT. */
            uint16_t pcrPid;
/** List of all the streams mentioned in the PMT. */
            PmtSection::StreamList streamList;
        };
/**
 *  \brief  A list of PMTs.
 */
        typedef std::list<PmtInfo> PmtInfoList;

    private:
        enum
        {
            // LCM of 188, 192 and 4096
            BUFFER_SIZE = 577536,
            VALID_PACKETS = 10
        };
        uint8_t* buffer;
        FILE* fileHandle;
        TsPacket* viewPacket;

        // File size in bytes
        uint64_t fileSize;
        // Number of bytes of data valid in the buffer currently
        uint64_t validBufferSize;
        // Current offset within the file for the start of the buffer
        uint64_t currentFileOffset;
        // File offset for the last packet fetched using any of the
        // viewPacket() calls
        uint64_t lastPacketOffset;
        // Size of the packets of the current TS
        uint8_t packetSize;
        // Indicated a valid TS file
        bool isTsFile;
        // Indicates EOF
        bool isEof;

        // PAT info
        PatInfo patInfo;
        // PMT info
        PmtInfoList pmtInfoList;

        void readFromOffset(uint64_t offset);
        void validate();
        void collectMetadata();

    public:
        TsFile();
        ~TsFile();

/**
 *  \brief  Opens a given file. To check if it is a valid TS file make use of
 *          isValid() instead.
 *  \param  fileName The filename can be absolute or relative and is passed
 *          as-is to fopen.
 *  \return true if file was opened successfully, false otherwise.
 */
        bool open(const char* fileName);
/**
 *  \brief  Close the file currently opened by the TsFile handle.
 */
        void close();
/**
 *  \brief  Check if the currently opened file is a valid TS file.
 *  \return valid TS file or not.
 */
        bool isValid();
/**
 *  \brief  View a TS packet by packet number.
 *          \warning The validity of the TsPacket handle is only till the
 *          next call to viewNextPacket(), viewPreviousPacket(), or
 *          viewPacketByNumber() or any other seek operations in TsFile.
 *          \warning To persist the TsPacket by allocating memory on the heap
 *          use the TsPacket::copy() method.
 *  \param  packetNumber Packet number from the beginning of the file.
 *  \return TsPacket handle of the packet number on success, NULL otherwise.
 */
        TsPacket* viewPacketByNumber(uint64_t packetNumber);
//      FIXME: add this functionality later
//        TsPacket* viewPacketByPid(uint16_t pid);
/**
 *  \brief  View the next TS packet with the reference being the packet
 *          number in the last call to viewPacketByNumber() /
 *          viewNextPacket() / viewPreviousPacket(), whichever was more recent.
 *          \warning The validity of the TsPacket handle is only till the
 *          next call to viewNextPacket(), viewPreviousPacket(), or
 *          viewPacketByNumber() or any other seek operations in TsFile.
 *          \warning To persist the TsPacket by allocating memory on the heap
 *          use the TsPacket::copy() method.
 *  \return TsPacket handle for the next packet, NULL otherwise.
 */
        TsPacket* viewNextPacket();
/**
 *  \brief  View the previous TS packet with the reference being the packet
 *          number in either the last call to viewPacketByNumber() /
 *          viewNextPacket() / viewPreviousPacket(), whichever was more recent.
 *          \warning The validity of the TsPacket handle is only till the
 *          next call to viewNextPacket(), viewPreviousPacket(), or
 *          viewPacketByNumber() or any other seek operations in TsFile.
 *          \warning To persist the TsPacket by allocating memory on the heap
 *          use the TsPacket::copy() method.
 *  \return TsPacket handle for the previous packet, NULL otherwise.
 */
        TsPacket* viewPreviousPacket();

/**
 *  \brief  Get the PAT info populated when opening the file.
 *  \return PAT Info.
 */
        const PatInfo& getPatInfo();
/**
 *  \brief  Get the PMT information list populated when opening the file.
 *  \return List of PMT Info(s).
 */
        const PmtInfoList& getPmtInfoList();
// FIXME: Implement it later
//        bool seek(uint64_t offset);
/**
 *  \brief  Get the file size.
 *  \return File size.
 */
        uint64_t getFileSize();
/**
 *  \brief  Get the size of the packets in the TS file.
 *  \return Packet size.
 */
        uint8_t getPacketSize();
};

inline uint64_t TsFile::getFileSize()
{
    return fileSize;
}

inline uint8_t TsFile::getPacketSize()
{
    return packetSize;
}

inline const TsFile::PatInfo& TsFile::getPatInfo()
{
    return patInfo;
}

inline const TsFile::PmtInfoList& TsFile::getPmtInfoList()
{
    return pmtInfoList;
}

#endif
