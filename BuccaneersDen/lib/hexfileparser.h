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
#ifndef HEXFILEPARSER_H
#define HEXFILEPARSER_H

#include <QFile>
#include <QByteArray>

class HEXFileParser
{
public:
    static const quint8 RecordMark = ':';
    enum RECTYP {Data = 0, EndOfFile = 1, ExtendedSegmentAR = 2, StartSegmentAR = 3, ExtendedLinearAR = 4, StartLinearAR = 5};

    HEXFileParser();

    int Parse(const QString &FileName, QByteArray &Memory);

private:
    bool VerifyChecksum(const QByteArray &Record);
};

#endif // HEXFILEPARSER_H
