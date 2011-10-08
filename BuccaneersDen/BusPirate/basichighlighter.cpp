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
#include "basichighlighter.h"

BASICHighlighter::BASICHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    QTextCharFormat basicKeywordFormat;
    basicKeywordFormat.setForeground(Qt::yellow);
    QStringList basicKeywordPatterns;
    basicKeywordPatterns << "\\bLET\\b" << "\\bIF\\b" << "\\bTHEN\\b"
                        << "\\bELSE\\b" << "\\bGOTO\\b" << "\\bGOSUB\\b"
                        << "\\bPRINT\\b" << "\\bINPUT\\b" << "\\bFOR\\b"
                        << "\\bTO\\b" << "\\bNEXT\\b" << "\\bREAD\\b"
                        << "\\bDATA\\b" << "\\bEND\\b";
    foreach (const QString &pattern, basicKeywordPatterns) {
             rule.pattern = QRegExp(pattern);
             rule.format = basicKeywordFormat;
             m_HighlightingRules.append(rule);
    }

    QTextCharFormat bpKeywordFormat;
    bpKeywordFormat.setForeground(Qt::blue);
    QStringList bpKeywordPatterns;
    bpKeywordPatterns << "\\bSTART\\b" << "\\bSTOP\\b" << "\\bSTARTR\\b"
                     << "\\bSTOPR\\b" << "\\bSEND\\b" << "\\bRECEIVE\\b"
                     << "\\bCLK\\b" << "\\bDAT\\b" << "\\bBITREAD\\b"
                     << "\\bADC\\b" << "\\bAUX\\b" << "\\bAUXPIN\\b"
                     << "\\bPSU\\b" << "\\bPULLUP\\b" << "\\bDELAY\\b"
                     << "\\bFREQ\\b" << "\\bDUTY\\b";
    foreach (const QString &pattern, bpKeywordPatterns) {
             rule.pattern = QRegExp(pattern);
             rule.format = bpKeywordFormat;
             m_HighlightingRules.append(rule);
    }

    QTextCharFormat lineNumberFormat;
    lineNumberFormat.setForeground(Qt::darkGray);
    rule.pattern = QRegExp("^[0-9]+");
    rule.format = lineNumberFormat;
    m_HighlightingRules.append(rule);

    QTextCharFormat sentCommentFormat;
    sentCommentFormat.setForeground(Qt::darkGreen);
    sentCommentFormat.setFontItalic(true);
    rule.pattern = QRegExp("^[0-9]+[ ]*REM[^\n]*");
    rule.format = sentCommentFormat;
    m_HighlightingRules.append(rule);

    QTextCharFormat unsentCommentFormat;
    unsentCommentFormat.setForeground(Qt::green);
    unsentCommentFormat.setFontItalic(true);
    rule.pattern = QRegExp("^REM[^\n]*");
    rule.format = unsentCommentFormat;
    m_HighlightingRules.append(rule);
    rule.pattern = QRegExp("^'[^\n]*");
    m_HighlightingRules.append(rule);
}

void BASICHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, m_HighlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
}
