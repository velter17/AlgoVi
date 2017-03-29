/**
 * Project   AlgoVi
 *
 * @file     ITerminal.hpp
 * @author   Dmytro Sadovyi
 * @date     27.03.2017
 */

#pragma once

#include <QObject>
#include <QString>

namespace NController
{

class ITerminal
{
public: // methods
    virtual void lock() = 0;
    virtual void unlock() = 0;
signals:
    virtual void command(const QString& cmd) = 0;
};

} // namespace NController
