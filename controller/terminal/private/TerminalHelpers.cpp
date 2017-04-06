/**
 * Project   AlgoVi
 *
 * @file     TerminalHelpers.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include "../TerminalHelpers.hpp"

namespace NController
{

QString colorize(const QString &text, QColor color)
{
   return "<font color=" + color.name() + ">" + text + "</font>";
}

QString convertTextToHtml(const QString &text)
{
   QString ret = text.toHtmlEscaped();
   ret.replace(QString("\n"), QString("<br>"));
   ret.replace(QString(" "), QString("&nbsp;"));
   return ret;
}

QString toHtmlSymbol(int code)
{
   return "&#" + QString::number(code) + ";";
}

} // namespace NController
