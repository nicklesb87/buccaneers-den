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
#ifndef PIC24_H
#define PIC24_H

#include <QtCore>

class Pic24MemoryImage {
public:
    virtual quint32 FlashSize() const = 0;
    virtual quint32 NbPages() const {
        return (FlashSize() / NbRowsInPage() / NbWordsInRow() / 2);
    };
    virtual quint32 BaseLoadAddress() const {
        return (FlashSize() - (Pic24DS30Placement * NbRowsInPage() * NbWordsInRow() * 2));
    };
    virtual quint32 NbRowsInPage() const {
        return Pic24NbRowsInPage;
    };
    virtual quint32 NbWordsInRow() const {
        return Pic24NbWordsInRow;
    };
    virtual quint32 WordSize() const {
        return Pic24WordSize;
    };
    virtual quint32 RowSize() const {
        return NbWordsInRow() * WordSize();
    };
    virtual quint32 PageSize() const {
        return NbRowsInPage() * NbWordsInRow() * WordSize();
    };

    quint32 RowAddress(quint32 Page, quint32 Row) const {
        return Page * PageSize() + Row * RowSize();
    };
    quint32 PageAddress(quint32 Page) const {
        return Page * PageSize();
    };

    bool IsPageUsed(quint32 PageNb) const;

    const QByteArray &GetMemoryImage() const {
        return m_MemoryImage;
    };

    void InitializeFromHexFile(const QString &HexFileName);

private:
    void ConvertHexImageToPicImage();
    void FixLoadAddress();

    static const quint32 Pic24NbRowsInPage = 8;
    static const quint32 Pic24NbWordsInRow = 64;
    static const quint32 Pic24WordSize = 3;
    static const quint32 Pic24DS30Placement = 1;

    QByteArray m_MemoryImage;
};

class Pic24fj64MemoryImage : public Pic24MemoryImage {
public:
    quint32 FlashSize() const {
        return FlashMemorySize;
    };

private:
    static const quint32 FlashMemorySize = 0xAC00;
};

class Pic24fj256MemoryImage : public Pic24MemoryImage {
public:
    quint32 FlashSize() const {
        return FlashMemorySize;
    };

private:
    static const quint32 FlashMemorySize = 0x2AC00;
};

#endif // PIC24_H
