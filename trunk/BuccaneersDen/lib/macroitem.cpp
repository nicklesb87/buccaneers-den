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
#include "macroitem.h"

#include <QVariant>

MacroItem::MacroItem(const QStringList &data, MacroItem *parent, int row)
{
    m_ParentItem = parent;
    if (m_ParentItem) {
        if (row == -1)
            m_ParentItem->appendChild(this);
        else
            m_ParentItem->insertChild(row, this);
    }
    m_ItemData = data;
}

MacroItem::~MacroItem()
{
    qDeleteAll(m_ChildItems);
}

void MacroItem::appendChild(MacroItem *child)
{
    m_ChildItems.append(child);
}

void MacroItem::insertChild(int row, MacroItem *child)
{
    m_ChildItems.insert(row, child);
}

void MacroItem::removeChild(MacroItem *child)
{
    m_ChildItems.removeOne(child);
}

void MacroItem::removeChildAt(int childNb)
{
    m_ChildItems.removeAt(childNb);
}

MacroItem *MacroItem::child(int row) const
{
    return m_ChildItems.value(row);
}

int MacroItem::childCount() const
{
    return m_ChildItems.count();
}

int MacroItem::columnCount() const
{
    return m_ItemData.count();
}

QVariant MacroItem::data(int column) const
{
    return m_ItemData.value(column);
}

void MacroItem::setName(QString Name)
{
    m_ItemData.replace(0, Name);
}

bool MacroItem::isFolder() const
{
    if (((m_ItemData.count() == 1) || m_ItemData[1].isEmpty()) && (QString("separator").compare(m_ItemData[0]) != 0))
        return true;

    return false;
}

bool MacroItem::isMacro() const
{
    if ((m_ItemData.count() > 1) && !m_ItemData[1].isEmpty())
        return true;

    return false;
}

bool MacroItem::isSeparator() const
{
    if (((m_ItemData.count() == 1) || m_ItemData[1].isEmpty()) && (QString("separator").compare(m_ItemData[0]) == 0))
        return true;

    return false;
}

int MacroItem::row() const
{
    if (m_ParentItem)
        return m_ParentItem->m_ChildItems.indexOf(const_cast<MacroItem*>(this));

    return 0;
}

MacroItem *MacroItem::parent() const
{
    return m_ParentItem;
}
