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
#ifndef MACROSMODEL_H
#define MACROSMODEL_H

#include "macroitem.h"

#include <QAbstractItemModel>
#include <QPalette>

class MacrosModel : public QAbstractItemModel
{
Q_OBJECT
public:

    static const int ColumnCount = 2;
    enum Column {NameColumn, CodeColumn};
    enum Role {SavingRole = Qt::UserRole};

    explicit MacrosModel(QObject *parent = 0);
    ~MacrosModel();

    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex addMacro(const QStringList &data, const QModelIndex &parent = QModelIndex(), int row = -1);
    void deleteMacro(QModelIndex index);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData *mimeData(const QModelIndexList &indexes) const;
    bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());

    void setPalette(const QPalette &palette);
    void disableSaving();
    void enableSaving();

signals:
    void NeedSaving(void);

private:
    MacroItem *nodeFromIndex(const QModelIndex &index) const;
    void AddMacroFromVnd(const QMimeData *data, int row, const QModelIndex &parent);
    void AddMacroFromPlainText(const QMimeData *data, int row, const QModelIndex &parent);

    MacroItem *m_RootItem;
    bool m_SavingEnabled;
    QPalette m_Palette;
};

#endif // MACROSMODEL_H
