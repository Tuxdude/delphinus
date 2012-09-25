/*
 *  TsFile.cpp - An abstraction for handling the raw Transport Stream
 *
 *  This file is part of delphinus.
 *
 *  Copyright (C) 2012 Ash (Tuxdude) <tuxdude.github@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as
 *  published by the Free Software Foundation; either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this program.  If not, see
 *  <http://www.gnu.org/licenses/>.
 */

#include "TsFile.h"
#include <cassert>
#include <list>
#include <set>

using namespace MpegConstants;

//#define DEBUG

#define MODULE_TS_FILE 0
#define CURRENT_MODULE MODULE_TS_FILE

#ifdef DEBUG
#define MSG(fmt, ...); LogOutput(CURRENT_MODULE, DelphinusUtils::LOG_INFO, " " fmt " \n", ##__VA_ARGS__);
#else
#define MSG(fmt, ...);
#endif

#define ERR(fmt, ...); LogOutput(CURRENT_MODULE, DelphinusUtils::LOG_ERROR, " " fmt " \n", ##__VA_ARGS__);

uint64_t readFile(uint8_t* buffer, FILE* fileHandle, uint64_t size);

uint64_t readFile(uint8_t* buffer, FILE* fileHandle, uint64_t size)
{
    uint64_t readSize;
    readSize = fread(buffer, 1, size, fileHandle);

    if (readSize == size)
    {
        MSG("Now read: %lu", readSize);
        return readSize;
    }

    assert(feof(fileHandle));
    MSG("%s", readSize > 0 ? "Partial Read" : "EOF!");
    return readSize;
}

void TsFile::readFromOffset(uint64_t offset)
{
    assert(offset % BUFFER_SIZE == 0);
    if (currentFileOffset != offset)
    {
        MSG("Gonna read from offset: %lu", offset);
        if (!fseeko(fileHandle, offset, SEEK_SET))
        {
            validBufferSize = readFile(buffer, fileHandle, BUFFER_SIZE);
            currentFileOffset = offset;
            //FIXME: Handle the EOF case
            isEof = (validBufferSize == 0);
            MSG("isEof: %d", isEof);
        }
        else
        {
            assert(false);
        }
    }
    else
    {
        MSG("Buffer already has the data - skipping file read");
    }
}

void TsFile::validate()
{
    readFromOffset(0);
    TsPacket tsPacket;
    uint64_t bufferOffset = 0;
    packetSize = 0;
    isTsFile = false;

    // Validate VALID_PACKETS number of TS packets
    if (!tsPacket.parse(buffer + bufferOffset, validBufferSize - bufferOffset))
    {
        return;
    }

    packetSize = tsPacket.getPacketSize();
    uint64_t maxValidBufferOffset = VALID_PACKETS * packetSize;
    assert (maxValidBufferOffset <= BUFFER_SIZE);
    bufferOffset += packetSize;

    while (bufferOffset < maxValidBufferOffset)
    {
        if (!tsPacket.parse(buffer + bufferOffset, validBufferSize - bufferOffset))
        {
            return;
        }
        bufferOffset += packetSize;
    }
    isTsFile = true;
}

void TsFile::collectMetadata()
{
    // Approach - Using section header's table ID based filtering (more efficient)
    // Start from packet 0
    // If all PIDs in PAT and PMT found and PID not in already found list:
    //      Check for sections when PUSI = 1
    //      If it is a section - check for Table IDs 0x00 and 0x02
    //      Then parse it - add PID to found list
    //
    std::set<uint16_t> pidsToFind;
    std::set<uint16_t> foundPids;
    patInfo.programList.clear();
    pmtInfoList.clear();

    uint64_t lastFileOffset = 0;
    readFromOffset(lastFileOffset);

    TsPacket tsPacket;
    pidsToFind.insert(PID_PAT);

    while(!isEof && pidsToFind.size() > 0)
    {
        uint8_t* data = buffer;
        uint64_t packetCount = 0;
        uint64_t maxPackets = validBufferSize / packetSize;
        uint64_t remainingData = validBufferSize;

        while (pidsToFind.size() > 0 && packetCount < maxPackets)
        {
//            MSG("Parsing packet: %lu", packetCount);
            if (!tsPacket.parse(data, remainingData))
            {
                ERR("Invalid TS packet");
                assert(false);
            }
            uint16_t pid = tsPacket.getPid();
            // PID is not NULL
            // PID is not in the already found list (FIXME: When sections are split ??)
            // Packet has payload and PUSI set to 1
            if (pid != PID_NULL && foundPids.find(pid) == foundPids.end() &&
                    tsPacket.hasPayload() && tsPacket.getPayloadUnitStartIndicator())
            {
                PesPacket pesPacket;
                pesPacket.parse(tsPacket.getPayload());
                if (pesPacket.getStartCodePrefix() != 0x000001)
                {
                    // If it is not a PES packet, it must be a section
                    PsiSection psiSection;
                    if (psiSection.parse(tsPacket.getPayload()))
                    {
                        MSG("Parsing Packet with a PSI Section PID: 0x%04x", pid);
                        // If it is a section, check the TableId
                        // The only ones we're interested in are PAT and PMT
                        if (psiSection.getTableId() == TABLE_PAT)
                        {
                            MSG("Found PAT");
                            PatSection patSection;
                            patSection.parse(tsPacket.getPayload(),
                                             tsPacket.getPacketSize() -
                                             psiSection.getDataOffset());
                            if (patSection.isCompleteSection())
                            {
                                MSG("complete PAT");
                                const PatSection::ProgramList& programList = patSection.getPrograms();
                                patInfo.programList = programList;
                                patInfo.packetNumber = (currentFileOffset / BUFFER_SIZE) + packetCount;
                                patInfo.transportStreamId = patSection.getTransportStreamId();
                                foundPids.insert(pid);
                                pidsToFind.erase(pid);
                                for (PatSection::ProgramList::const_iterator ix = programList.begin();
                                     ix != programList.end(); ++ix)
                                {
                                    pidsToFind.insert(ix->pmtPid);
                                }
                            }
                        }
                        else if (psiSection.getTableId() == TABLE_PMT)
                        {
                            MSG("Found PMT");
                            PmtSection pmtSection;
                            pmtSection.parse(tsPacket.getPayload(),
                                    tsPacket.getPacketSize() -
                                    psiSection.getDataOffset());
                            if (pmtSection.isCompleteSection())
                            {
                                MSG("complete PMT");
                                const PmtSection::StreamList& streamList = pmtSection.getStreamList();
                                PmtInfo pmtInfo;
                                pmtInfo.packetNumber = (currentFileOffset / BUFFER_SIZE) + packetCount;
                                pmtInfo.pmtPid = pid;
                                pmtInfo.programNumber = pmtSection.getProgramNumber();
                                pmtInfo.pcrPid = pmtSection.getPcrPid();
                                pmtInfo.streamList = streamList;
                                pmtInfoList.push_back(pmtInfo);
                                foundPids.insert(pid);
                                pidsToFind.erase(pid);
                            }
                        }
                    }
                }
            }
            packetCount += 1;
            data += packetSize;
            remainingData -= packetSize;
        }
        if (pidsToFind.size() > 0)
        {
            // We have either reached the end of the buffer and need to read
            // the next bytes of the file into the buffer (or)
            // we have reached the EOF - FIXME
            if (!isEof)
            {
                lastFileOffset += BUFFER_SIZE;
                readFromOffset(lastFileOffset);
            }
        }
    }
}

TsFile::TsFile()
    :   
        buffer(NULL),
        fileHandle(NULL),
        viewPacket(NULL),
        fileSize(0),
        validBufferSize(0),
        currentFileOffset((uint64_t) - 1),
        lastPacketOffset((uint64_t) - 1),
        packetSize(0),
        isTsFile(false),
        isEof(true)
{
    buffer = new uint8_t[BUFFER_SIZE];
    assert(buffer != NULL);
    viewPacket = new TsPacket();
    assert(viewPacket != NULL);
}

TsFile::~TsFile()
{
    if (viewPacket)
    {
        delete viewPacket;
        viewPacket = NULL;
    }
    close();
    if (buffer)
    {
        delete[] buffer;
        buffer = NULL;
    }
}

bool TsFile::open(const char* fileName)
{
    close();
    fileHandle = fopen(fileName, "rb");
    if (fileHandle == NULL)
    {
        return false;
    }
    fseeko(fileHandle, 0, SEEK_END);
    fileSize = ftello(fileHandle);
    fseeko(fileHandle, 0, SEEK_SET);
    lastPacketOffset = (uint64_t) - 1;
    isEof = (fileSize == 0);

    validate();
    collectMetadata();

    return true;
}

void TsFile::close()
{
    if (fileHandle)
    {
        fclose(fileHandle);
        fileHandle = NULL;
        fileSize = 0;
        validBufferSize = 0;
        packetSize = 0;
        currentFileOffset = (uint64_t) - 1;
        lastPacketOffset = (uint64_t) - 1;
        isEof = true;
    }
}

bool TsFile::isValid()
{
    return isTsFile; 
}

TsPacket* TsFile::viewPacketByNumber(uint64_t packetNumber)
{
    uint64_t packetOffset = packetNumber * packetSize;
    uint64_t bufferOffset = packetOffset % BUFFER_SIZE;
    if (packetOffset >= fileSize)
    {
        // invalid packet number - exceeds file size
        return NULL;
    }
    if (currentFileOffset > packetOffset ||
        currentFileOffset + BUFFER_SIZE < packetOffset)
    {
        readFromOffset(packetOffset - bufferOffset);
    }

    MSG("Returning packet %lu from buffer offset: %lu", packetNumber, bufferOffset);
    viewPacket->parse(buffer + bufferOffset, validBufferSize - bufferOffset);
    lastPacketOffset = packetOffset;
    return viewPacket;
}

TsPacket* TsFile::viewNextPacket()
{
    uint64_t packetOffset = (uint64_t) - 1;
    if (lastPacketOffset == (uint64_t) - 1)
    {
        packetOffset = 0;
    }
    else if (lastPacketOffset + packetSize >= fileSize)
    {
        // reached EOF
        return NULL;
    }
    
    packetOffset = lastPacketOffset + packetSize;
    uint64_t bufferOffset = packetOffset % BUFFER_SIZE;
    if (bufferOffset == 0)
    {
        // We do not have this packet in the buffer yet
        readFromOffset(packetOffset);
    }
    MSG("Returning packet from buffer offset: %lu", bufferOffset);
    viewPacket->parse(buffer + bufferOffset, validBufferSize - bufferOffset);
    lastPacketOffset = packetOffset;
    return viewPacket;
}

TsPacket* TsFile::viewPreviousPacket()
{
    uint64_t packetOffset = (uint64_t) - 1;
    if (lastPacketOffset == (uint64_t) - 1)
    {
        packetOffset = 0;
    }
    else if (lastPacketOffset - packetSize >= fileSize)
    {
        // This case wont be hit - if we do then detonate
        assert(false);
        return NULL;
    }
    
    packetOffset = lastPacketOffset - packetSize;
    uint64_t bufferOffset = packetOffset % BUFFER_SIZE;
    if (bufferOffset == (uint64_t)(BUFFER_SIZE - packetSize))
    {
        // We do not have this packet in the buffer yet
        readFromOffset(packetOffset - bufferOffset);
    }
    viewPacket->parse(buffer + bufferOffset, validBufferSize - bufferOffset);
    lastPacketOffset = packetOffset;
    return viewPacket;
}

