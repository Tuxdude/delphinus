/*
 *  tsinfo.cpp - A program to parse the info from a MPEG-2 Transport Stream
 *  and display the details.
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

#include "libdelphinus/TsFile.h"
#include "libdelphinus/Pes.h"
#include "libdelphinus/PsiTables.h"
#include <cassert>

#define DEBUG

#ifdef DEBUG
#define MSG(x, ...); ::fprintf(stderr, " " x " \n", ##__VA_ARGS__);
#else
#define MSG(x, ...); 
#endif

#define ERR(x, ...); ::fprintf(stderr, " " x " \n", ##__VA_ARGS__);

void printUsage(char* programName);

void printUsage(char* programName)
{
  ERR("Usage: %s <FILE>", programName);
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printUsage(argv[0]);
        return -1;
    }

    TsFile tsFile;
    if (!tsFile.open(argv[1]))
    {
        ERR("Unable to open the file: %s", argv[1]);
        return -1;
    }

    if (!tsFile.isValid())
    {
        ERR("Not a valid TS file");
        return -1;
    }

    MSG("-----------------------------------------------------------");
    MSG("File size: %" PRIu64 " bytes", tsFile.getFileSize());
    MSG("-----------------------------------------------------------");

    uint64_t packetCount = 0;
    TsPacket* tsPacket = NULL;
    tsPacket = tsFile.viewPacketByNumber(0);
    PesPacket pesPacket;
    while (packetCount < 50000)
    {
        uint16_t pid = tsPacket->getPid();
//        MSG("PID: 0x%03x", pid);
        if (pid != TsPacket::PID_NULL)
        {
#if 0
            MSG("Packet: %" PRId64 "", packetCount);
            MSG("Sync byte: 0x%02x", tsPacket->getSyncByte());
            MSG("TEI: 0x%01x", tsPacket->getTransportErrorIndicator());
            MSG("PUSI: 0x%01x", tsPacket->getPayloadUnitStartIndicator());
            MSG("Transport Priority: 0x%01x", tsPacket->getTransportPriority());
            MSG("PID: 0x%03x", pid);
            MSG("Scrambling Control: 0x%01x", tsPacket->getTransportScramblingControl());
            MSG("Adaptation Field Control: 0x%01x", tsPacket->getAdaptationFieldControl());
            MSG("Continuity Counter: 0x%01x", tsPacket->getContinuityCounter());
#endif
            if (tsPacket->hasPayload() && tsPacket->getPayloadUnitStartIndicator())
            {
                pesPacket.parse(tsPacket->getPayload());
                if (pesPacket.getStartCodePrefix() != 0x000001)
                {
                    // It must be a section
                    PsiSection psiSection;
#if 1
                    if (psiSection.parse(tsPacket->getPayload()))
                    {
                        if (psiSection.getTableId() == PsiSection::TABLE_PAT)
                        {
                            PatSection patSection;
                            uint16_t dataSize = tsPacket->getPacketSize() - tsPacket->getPayloadOffset() -
                                1 - psiSection.getPointerField();
                            patSection.parse(tsPacket->getPayload(), dataSize);
                            if (patSection.isCompleteSection())
                            {
                                MSG("PID: 0x%03x", tsPacket->getPid());
                                const PatSection::ProgramList& programs = patSection.getPrograms();
                                for (PatSection::ProgramList::const_iterator ix = programs.begin();
                                     ix != programs.end(); ++ix)
                                {
                                    MSG("Program Number: %u PMT PID: 0x%03x",
                                        ix->programNumber, ix->pmtPid);
                                }
                            }
                            else
                            {
                                // We either dont have the complete PAT section yet
                                // or it is an invalid PAT section
                                assert(false);
                            }
                        }
                        else if (psiSection.getTableId() == PsiSection::TABLE_PMT)
                        {
                            PmtSection pmtSection;
                            uint16_t dataSize = tsPacket->getPacketSize() - tsPacket->getPayloadOffset() -
                                1 - psiSection.getPointerField();
                            pmtSection.parse(tsPacket->getPayload(), dataSize);
                            if (pmtSection.isCompleteSection())
                            {
                                MSG("----------------------------------------------------------------");
                                MSG("PID: 0x%03x", tsPacket->getPid());
                                MSG("Program Number: %u", pmtSection.getProgramNumber());
                                MSG("PCR PID: 0x%03x", pmtSection.getPcrPid());
                                const PmtSection::StreamList& streams = pmtSection.getStreamList();
                                for (PmtSection::StreamList::const_iterator ix = streams.begin();
                                     ix != streams.end(); ++ix)
                                {
                                    MSG("PID: 0x%02x StreamType: 0x%02x %s",
                                        ix->pid, ix->streamType,
                                        PmtSection::getStreamTypeStr(ix->streamType));
                                }
                                MSG("----------------------------------------------------------------");
                            }
                            else
                            {
                                // We either dont have the complete PMT section yet
                                // or it is an invalid PMT section
                                assert(false);
                            }
                        }
                    }
#endif
#if 0
                    if (psiSection.parse(tsPacket->getPayload()) &&
                        psiSection.getSectionNumber() != psiSection.getLastSectionNumber())
                    {
                        MSG("Packet: %" PRId64 "", packetCount);
                        MSG("PID: 0x%03x", pid);
                        MSG("Table Id: 0x%02x", psiSection.getTableId());
                        MSG("Pointer field: 0x%02x", psiSection.getPointerField());
                        MSG("Section length: 0x%03x", psiSection.getLength());
                        MSG("-----------------------------------------------------------");
                    }
#endif
                }
//                MSG("hasAdaptation: %d PES Start Code: 0x%08x",
//                    tsPacket->hasAdaptationField(), pesPacket.getStartCodePrefix());
//                MSG("streamId: 0x%02x", pesPacket.getStreamId());
//                MSG("PES packet length: 0x%02x", pesPacket.getLength());
            }
//            MSG("-----------------------------------------------------------");
        }
        tsPacket = tsFile.viewNextPacket();
        ++packetCount;
    }

    return 0;
}

