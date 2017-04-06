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
signals:
    virtual void log(const QString& str) = 0;
    virtual void error(const QString& str) = 0;
    virtual void logHtml(const QString& str) = 0;
};

} // namespace NCommand

