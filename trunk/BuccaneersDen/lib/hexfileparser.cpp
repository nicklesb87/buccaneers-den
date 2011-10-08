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
#include "hexfileparser.h"

#include <QtCore>

HEXFileParser::HEXFileParser()
{

}

bool HEXFileParser::VerifyChecksum(const QByteArray &Record)
{
    qint8 Total = 0;
    for (int i = 0; i < Record.count(); i++)
        Total += static_cast<qint8>(Record[i]);

    return (Total == 0)? true: false;
}

int HEXFileParser::Parse(const QString &FileName, QByteArray &Memory)
{
    QFile HexFile(FileName);
    if (!HexFile.open(QIODevice::ReadOnly))
        return -1;

    quint32 LinearBaseAddr = 0;
    while (!HexFile.atEnd()) {
        QByteArray CurrentLine = HexFile.readLine(512);
        if (static_cast<quint8>(CurrentLine[0]) != RecordMark) {
            qDebug() << "Not a record line";
            continue;
        }

        QByteArray CurrentRecord = QByteArray::fromHex(CurrentLine);

        quint8 RecordLength = CurrentRecord.at(0);
        quint32 LoadOffset = (static_cast<quint32>(static_cast<quint8>(CurrentRecord.at(1))) << 8) | static_cast<quint8>(CurrentRecord.at(2));
        quint8 RecordType = CurrentRecord.at(3);

        switch(RecordType) {
        case Data:
        {
            if (!VerifyChecksum(CurrentRecord))
                qDebug() << "Checksum doesn't match";

            CurrentRecord.remove(0, 4);
            CurrentRecord.remove(CurrentRecord.count()-1,1);
            Memory.replace(LinearBaseAddr | LoadOffset, RecordLength, CurrentRecord);
            break;
        }
        case EndOfFile:

            break;
        case ExtendedSegmentAR:

            break;
        case StartSegmentAR:

            break;
        case ExtendedLinearAR:
            if (!VerifyChecksum(CurrentRecord))
                qDebug() << "Checksum doesn't match";

            LinearBaseAddr = (static_cast<quint32>(static_cast<quint8>(CurrentRecord.at(4))) << 24) | (static_cast<quint32>(static_cast<quint8>(CurrentRecord.at(5))) << 16);
            break;
        case StartLinearAR:

            break;
        default:
            qDebug() << "Invalid Record Type";
            break;
        }
    }

    HexFile.close();
    return 0;
}
