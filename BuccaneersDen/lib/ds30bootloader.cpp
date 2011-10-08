/******************************************************************************
 *
 * This file is part of Buccaneer's Den.
 *
 * Copyright (C) 2011 Vincent Roy http://code.google.com/p/buccaneers-den/
 *
 * Buccaneer's Den is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Buccaneer's Den is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Buccaneer's Den.  If not, see <http://www.gnu.org/licenses/>.
 *
 *****************************************************************************/
#include "ds30bootloader.h"
#include <abstractserial.h>
#include "picmemoryimage.h"

#define COMPARE_WITH_PIRATE_LOADER

DS30BootLoader::DS30BootLoader(AbstractSerial *SerialPort, QObject *parent) :
    QObject(parent)
{
    m_SerialPort = SerialPort;
}

bool DS30BootLoader::SendFirmware(const Pic24MemoryImage *PicMemory)
{
    if (!IsDeviceReady())
        return false;

    DS30Command Command;
    emit FirmwareUpgradeNbStep(PicMemory->NbPages()-1);
    for(quint32 PageNb=0; PageNb < PicMemory->NbPages(); PageNb++) {
        emit FirmwareUpgradeStep(PageNb);
        if (!PicMemory->IsPageUsed(PageNb))
            continue;

        quint32 CurrentAddress = PageNb * (PicMemory->NbRowsInPage() * PicMemory->NbWordsInRow() * 2);
        Command.PrepareErasePage(CurrentAddress);
        if(SendCommand(Command) < 0 ) {
            return false;
        }

#ifdef COMPARE_WITH_PIRATE_LOADER
        printf("Erasing page %d, %04x...", PageNb, CurrentAddress);
        puts("OK");
#endif

        for(quint32 RowNb = 0; RowNb < PicMemory->NbRowsInPage(); RowNb++, CurrentAddress += (PicMemory->NbWordsInRow() * 2)) {
            Command.PrepareWriteRow(CurrentAddress, PicMemory->GetMemoryImage().mid(PicMemory->RowAddress(PageNb, RowNb), PicMemory->RowSize()), PicMemory);
#ifdef COMPARE_WITH_PIRATE_LOADER
            printf("Writing page %d row %d, %04x...", PageNb, RowNb + PageNb*PicMemory->NbRowsInPage(), CurrentAddress);
            puts("OK");
#endif

            if(SendCommand(Command) < 0 ) {
                return false;
            }
        }
    }

    return true;
}

bool DS30BootLoader::IsDeviceReady()
{
    m_SerialPort->write(QByteArray(1, HELLO));
    m_SerialPort->flush();
    if (!m_SerialPort->waitForReadyRead(5000)) {
        m_ErrorTitle = QObject::tr("Timeout in Firmware Update");
        m_ErrorMessage = QObject::tr("Timeout when triying to enter firmware update mode.");
        return false;
    }
    QByteArray Response = m_SerialPort->readAll();
    if (static_cast<quint8>(Response.at(3)) != OK) {
        m_ErrorTitle = QObject::tr("Wrong answer from bootloader");
        m_ErrorMessage = QObject::tr("Bootloader is not ok toenter firmware update mode.");
        return false;
    }
    return true;
}

#ifdef COMPARE_WITH_PIRATE_LOADER
void dumpHex(const QByteArray &buf)
{
    for(int i=0; i < buf.count(); i++){
        printf("%02X ", static_cast<quint8>(buf.at(i)));
    }
    putchar('\n');
}
#endif

int DS30BootLoader::SendCommand(const DS30Command &Command)
{
#ifdef COMPARE_WITH_PIRATE_LOADER
    dumpHex(Command);
#else
    m_SerialPort->write(Command, Command.length());
    m_SerialPort->flush();
    if (!m_SerialPort->waitForReadyRead(5000)) {
        m_ErrorTitle = QObject::tr("Timeout in Firmware Update");
        m_ErrorMessage = QObject::tr("Timeout when waiting for answer to command.");
        return -1;
    }
    QByteArray byteArray = m_SerialPort->readAll();
    if (static_cast<quint8>(byteArray.at(0)) == OK) {
        return 0;
    } else if (static_cast<quint8>(byteArray.at(0)) == PROTECTION) {
        qDebug("Command skipped by bootloader");
        return 0;
    } else {
        m_ErrorTitle = QObject::tr("Wrong answer from bootloader");
        m_ErrorMessage = QObject::tr("Bootloader is not ok with command.");
        return -1;
    }
#endif
    return 0;
}

void DS30Command::PrepareErasePage(quint32 Address)
{
    clear();
    SetAddress(Address);
    (*this)[COMMAND_OFFSET] = ERASE_COMMAND;
    (*this)[LENGTH_OFFSET] = 1;
    (*this)[PAYLOAD_OFFSET] = ComputeCRC();
}

void DS30Command::PrepareWriteRow(quint32 Address, const QByteArray &Data, const Pic24MemoryImage *PicDetails)
{
    clear();
    SetAddress(Address);
    (*this)[COMMAND_OFFSET] = WRITE_COMMAND;
    (*this)[LENGTH_OFFSET] = PicDetails->RowSize() + 1;
    insert(PAYLOAD_OFFSET, Data);
    (*this)[PAYLOAD_OFFSET + PicDetails->RowSize()] = ComputeCRC();
}

quint8 DS30Command::ComputeCRC()
{
    quint8 Length = HEADER_LENGTH + (*this)[LENGTH_OFFSET]-1;
    quint8 crc = 0;
    for(quint8 i = 0; i < Length; i++) {
        crc -= (*this)[i];
    }
    return crc;
}
