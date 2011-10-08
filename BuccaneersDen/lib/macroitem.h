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
#ifndef MACROITEM_H
#define MACROITEM_H

#include <QObject>
#include <QStringList>

class MacroItem
{
public:
    MacroItem(const QStringList &data, MacroItem *parent = 0, int row = -1);
    ~MacroItem();

    void appendChild(MacroItem *child);
    void insertChild(int row, MacroItem *child);
    void removeChild(MacroItem *child);
    void removeChildAt(int childNb);

    MacroItem *child(int row) const;
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    void setName(QString Name);
    int row() const;
    MacroItem *parent() const;

    bool isFolder() const;
    bool isMacro() const;
    bool isSeparator() const;

private:
    QList<MacroItem*> m_ChildItems;
    QStringList m_ItemData;
    MacroItem *m_ParentItem;
};

#endif // MACROITEM_H
