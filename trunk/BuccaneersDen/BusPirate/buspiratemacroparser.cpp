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
#include "buspiratemacroparser.h"

#include <QtCore>

bool BusPirateMacroParser::Parse(const QString &Macro, QByteArray &Result)
{
    bool IsBinary = false;
    QRegExp ByteRX("^\\s*bytes");
    ByteRX.setCaseSensitivity(Qt::CaseInsensitive);
    QRegExp BasicRX("^\\s*basic");
    BasicRX.setCaseSensitivity(Qt::CaseInsensitive);
    if (ByteRX.indexIn(Macro) != -1) {
        ParseBinaryCommand(Macro.mid(ByteRX.matchedLength()), Result);
        IsBinary = true;
    } else if (BasicRX.indexIn(Macro) != -1) {
        ParseBasicCommand(Macro.mid(ByteRX.matchedLength()), Result);
        if ((Result.at(Result.count()-1) != '\n') ||
            (Result.at(Result.count()-1) != '\r'))
            Result.append('\n');
    } else {
        Result = Macro.toAscii();
        if (Result.isEmpty() ||
            (Result.at(Result.count()-1) != '\n') ||
            (Result.at(Result.count()-1) != '\r'))
            Result.append('\n');
    }
    return IsBinary;
}

void BusPirateMacroParser::ParseBinaryCommand(const QString &Macro, QByteArray &Result)
{
    int pos = 0;
    QRegExp number("(?:(\\d*)(?:\\s*\\*\\s*))?(0x|b)([0-9A-Fa-f]+)");
    while ((pos = number.indexIn(Macro, pos)) != -1) {
        int Multiplier = number.cap(1).toLong();
        Multiplier = (Multiplier) ? Multiplier: 1;
        unsigned char current = 0;
        if (number.cap(2) == "0x") {
            current = number.cap(3).toULong(0, 16);
        } else if (number.cap(2) == "b") {
            current = number.cap(3).toULong(0, 2);
        } else {
            current = number.cap(3).toULong(0, 10);
        }

        for(int i = 0; i < Multiplier; i++) {
            Result.push_back(current);
        }

        pos += number.matchedLength();
    }
}

void BusPirateMacroParser::ParseBasicCommand(const QString &Macro, QByteArray &Result)
{
    QRegExp remComment("^REM[^\n]*");
    QRegExp caretComment("^'[^\n]*");
    QStringList lines = Macro.split("\n");
    QStringList::const_iterator constIterator;
    for (constIterator = lines.constBegin(); constIterator != lines.constEnd(); ++constIterator) {
        if ((remComment.indexIn(*constIterator) != -1) && (caretComment.indexIn(*constIterator) != -1))
            Result += (*constIterator).toAscii();
    }

}
