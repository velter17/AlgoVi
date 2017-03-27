/**
 * Project   AlgoVi
 *
 * @file     ITerminalCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     27.03.2017
 */

#pragma once

#include <QString>
#include <QStringList>

#include "framework/commands/ITerminalSignals.hpp"

namespace NCommand
{

/**
 * Interface of terminal command
 */
class ITerminalCommand : public ITerminalSignals
{
public: // methods
    virtual ~ITerminalCommand() = 0;

public slots:
    virtual void run() = 0;
    virtual void appendData(const QString& str) = 0;
    virtual void terminate() = 0;

    virtual void setWorkingDirectory(const QString& dir) = 0;
    virtual void setWorkingTime(uint32_t time) = 0;

signals:
    void started();
    void finished(int code);

protected: // methods
    virtual QString getManualMessage() = 0;
};

} // namespace NCommand

