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
#ifndef MACROSVIEW_H
#define MACROSVIEW_H

#include <QTreeView>

class MacrosView : public QTreeView
{
Q_OBJECT
public:
    explicit MacrosView(QWidget *parent = 0);
    void contextMenuEvent(QContextMenuEvent *event);

protected:


signals:
    void macroActivated(QString);

public slots:
    void activateMacro(void);
    void createFolder(void);
    void deleteMacro(void);

private slots:
    void itemActivated(QModelIndex index);

private:
    QPoint m_StartPos;
};

#endif // MACROSVIEW_H
