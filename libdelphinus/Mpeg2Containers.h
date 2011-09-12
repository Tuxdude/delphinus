/*
 *  Mpeg2Containers.h - abstraction for MPEG-2 containers
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

#ifndef DELPHINUS_MPEG2CONTAINERS_H
#define DELPHINUS_MPEG2CONTAINERS_H
#include <inttypes.h>

// view lets you view the packet in buffer, and the returned pointer
// might not be valid after a next call to viewPacket
// Use copyPacket of TransportStreamPacket to make the packet permanent
// get on the other hand allocates memory explicitly for the packet's data
// while returning, so it can be kept until the app frees up the memory.

class TsFile : public FileInterface
{
    TransportStreamPacket* viewPacketByNumber(uint64_t packetNumber);
    TransportStreamPacket* viewPacketByPid(uint16_t pid);
    TransportStreamPacket* viewNextPacket();
    TransportStreamPacket* viewPreviousPacket();
    TransportStreamPacket* getPacketByNumber(uint64_t packetNumber);
    TransportStreamPacket* getPacketByPid(uint16_t pid);
    TransportStreamPacket* getNextPacket();
    TransportStreamPacket* getPreviousPacket();

    ProgramInfo* getProgramInfo();
    NetworkInfo* getNetworkInfo();
    bool seek(uint64_t offset);
    uint64_t getPacketCount();
    uint8_t getPacketSize();
};

class PesPacket
{
    void clear();
    void appendPartialPes(uint8_t* data);
    void parse(uint8_t* data);
    bool isValid();
    uint32_t getLength();
};

class PsFile : public FileInterface
{
    ProgramStreamPacket* viewPacketByNumber(uint64_t packetNumber);
    ProgramStreamPacket* viewPacketByPid(uint16_t streamId);
    ProgramStreamPacket* viewNextPacket();
    ProgramStreamPacket* viewPreviousPacket();
    ProgramStreamPacket* getPacketByNumber(uint64_t packetNumber);
    ProgramStreamPacket* getPacketByPid(uint16_t streamId);
    ProgramStreamPacket* getNextPacket();
    ProgramStreamPacket* getPreviousPacket();

    ProgramInfo* getProgramInfo();
    bool seek(uint64_t offset);
    uint64_t getPacketCount();
    uint8_t getPacketSize();
};


