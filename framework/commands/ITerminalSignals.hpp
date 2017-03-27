/**
 * Project   AlgoVi
 *
 * @file     ITerminalSignals.hpp
 * @author   Dmytro Sadovyi
 * @date     27.03.2017
 */

#pragma once

#include <QObject>
#include <QString>

namespace NCommand
{

class ITerminalSignals : public QObject
{
    Q_OBJECT
signals:
    void log(const QString& str);
    void error(const QString& str);
    void logHtml(const QString& str);
};

} // namespace NCommand

