/*
 *  TsFile.h - An abstraction for handling the raw Transport Stream
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

#ifndef DELPHINUS_TSFILE_H
#define DELPHINUS_TSFILE_H
#include <cstdio>
#include "Ts.h"

// view lets you view the packet in buffer, and the returned pointer
// might not be valid after a next call to viewPacket
// Use copy() method of TsPacket to make the packet permanent in memory

class TsFile
{
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

        uint64_t fileSize;
        uint64_t validBufferSize;
        uint64_t currentFileOffset;
        uint64_t lastPacketOffset;
        uint8_t packetSize;
        bool isTsFile;

        void readFromOffset(uint64_t offset);
        void validate();

    public:
        TsFile();
        ~TsFile();

        // Opens a given file and checks if it is valid
        bool open(const char* fileName);
        // Closes the currently open file
        void close();
        // Checks and returns if the last opened file is valid or not
        bool isValid();
        // View the packet - call to any of the next view function may
        // make this memory invalid
        // Caller should not free up the memory returned, TsFile takes
        // care of freeing up the memory
        // The packet can be persisted in the memory by calling the copy()
        // method of TsPacket
        TsPacket* viewPacketByNumber(uint64_t packetNumber);
//        TsPacket* viewPacketByPid(uint16_t pid);
        TsPacket* viewNextPacket();
        TsPacket* viewPreviousPacket();

//        ProgramInfo* getProgramInfo();
//        NetworkInfo* getNetworkInfo();
//        bool seek(uint64_t offset);
        uint64_t getFileSize();
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

#endif
