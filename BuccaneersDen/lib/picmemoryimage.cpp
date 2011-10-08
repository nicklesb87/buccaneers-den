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
#include "picmemoryimage.h"
#include "hexfileparser.h"

void Pic24MemoryImage::InitializeFromHexFile(const QString &HexFileName)
{
    m_MemoryImage.clear();
    m_MemoryImage.fill(0xFF, FlashSize()*4/WordSize());
    HEXFileParser Parser;
    Parser.Parse(HexFileName, m_MemoryImage);
    ConvertHexImageToPicImage();
    FixLoadAddress();
}

void Pic24MemoryImage::ConvertHexImageToPicImage()
{
    QByteArray Firmware(FlashSize(), 0xFF);
    int FirmwareIndex = 0;
    for (int i = 0; i < m_MemoryImage.count(); i+=4) {
        Firmware[FirmwareIndex++] = m_MemoryImage[i+2];
        Firmware[FirmwareIndex++] = m_MemoryImage[i];
        Firmware[FirmwareIndex++] = m_MemoryImage[i+1];
    }
    m_MemoryImage.clear();
    m_MemoryImage = Firmware;
}

void Pic24MemoryImage::FixLoadAddress()
{
    quint32 BLAddress = BaseLoadAddress();
    quint32 GotoUserAppAdress = BLAddress - 4;
    quint32 GotoUserAppAdressB3 = (GotoUserAppAdress / 2) * 3;
    for (quint32 iIter = 0; iIter < 6; iIter++ ) {
        m_MemoryImage[GotoUserAppAdressB3 + iIter] = m_MemoryImage[iIter];
    }
    m_MemoryImage[0] = 0x04;
    m_MemoryImage[1] = ((BLAddress & 0x0000FE));
    m_MemoryImage[2] = ((BLAddress & 0x00FF00) >> 8);
    m_MemoryImage[3] = 0x00;
    m_MemoryImage[4] = ((BLAddress & 0x7F0000) >> 16);
    m_MemoryImage[5] = 0x00;
};

bool Pic24MemoryImage::IsPageUsed(quint32 PageNb) const
{
    bool PageInUse = false;
    for (quint32 i = PageAddress(PageNb); i < PageAddress(PageNb) + PageSize(); i++) {
        if (i >= static_cast<quint32>(GetMemoryImage().count())) {
            PageInUse = false;
            break;
        }
        if (static_cast<quint8>(GetMemoryImage().at(i)) != 0xFF) {
            PageInUse = true;
            break;
        }
    }
    return PageInUse;
}
