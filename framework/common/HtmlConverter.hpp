/**
 * Project   AlgoVi
 *
 * @file     HtmlConverter.hpp.hpp
 * @author   Dmytro Sadovyi
 * @date     15.05.2017
 */

#pragma once

#include <QString>
#include <QMap>


namespace NCommon
{

inline QString colorize(const QString &text, const QString& color)
{
   return "<font color=" + color + ">" + text + "</font>";
}

inline QString toHtmlSymbol(int code)
{
   return "&#" + QString::number(code) + ";";
}

inline QString convertToHtml(QChar c)
{
    static QMap <QChar, QString> mp{
        {'\n', "<br>"},
        {' ', "&nbsp;"}
    };
    if(mp.find(c) == mp.end())
    {
        return QString(c).toHtmlEscaped();
    }
    return mp[c];
}

inline QString convertToHtml(const QString& text)
{
    QString ret;
    for(QChar c : text)
    {
        ret += convertToHtml(c);
    }
    return ret;
}

} // namespace NCommon

