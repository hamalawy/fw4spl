/* ***** BEGIN LICENSE BLOCK *****
 * FW4SPL - Copyright (C) IRCAD, 2009-2015.
 * Distributed under the terms of the GNU Lesser General Public License (LGPL) as
 * published by the Free Software Foundation.
 * ****** END LICENSE BLOCK ****** */

#ifndef __FWGUIQT_HIGHLIGHTER_CPPHIGHLIGHTER_HPP__
#define __FWGUIQT_HIGHLIGHTER_CPPHIGHLIGHTER_HPP__

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

#include "fwGuiQt/config.hpp"

class QTextDocument;

namespace fwGuiQt
{
namespace highlighter
{

class FWGUIQT_CLASS_API CppHighlighter : public QSyntaxHighlighter
{
Q_OBJECT

public:
    FWGUIQT_API CppHighlighter(QTextDocument *parent = 0);

protected:
    FWGUIQT_API void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat multiLineCommentFormat;

};

} //namespace fwGuiQt
} //namespace highlighter

#endif // __FWGUIQT_HIGHLIGHTER_CPPHIGHLIGHTER_HPP__
