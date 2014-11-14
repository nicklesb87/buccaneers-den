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
#ifndef BUSPIRATECONFIG_H
#define BUSPIRATECONFIG_H

#include <QDialog>

class QLineEdit;
class QComboBox;

class BusPirateConfig : public QDialog
{
    Q_OBJECT
public:
    explicit BusPirateConfig(QWidget *parent = 0);
    void SetCurrentDevice(const QString &DeviceName);

signals:
    void ConfigChanged(void);

public slots:

private slots:
    void BaudsChanged(int index);
    void ChooseFont(void);
    void ChooseTextColor(void);
    void ChooseBackgroundColor(void);
    void MaxCmdLengthChanged(QString NewMaximum);

private:
    void InitBaudsBox(const QString &DeviceName);
    QLineEdit *m_TextExample;
    QPushButton *m_ChangeFont;
    QPushButton *m_ChangeForeground;
    QPushButton *m_ChangeBackground;
    QComboBox *m_PossibleBauds;
    QLineEdit *m_MaxCmdLength;
    QString m_CurrentDeviceName;
};

#endif // BUSPIRATECONFIG_H
