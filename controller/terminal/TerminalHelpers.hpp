/**
 * Project   AlgoVi
 *
 * @file     TerminalHelpers.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <QString>
#include <QColor>

namespace NController
{

QString convertTextToHtml(const QString& text);
QString colorize(const QString& text, QColor color);
QString toHtmlSymbol(int code);
int calcLCP(const QStringList& list);


} // namespace NController

