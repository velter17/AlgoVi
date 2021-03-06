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
#include "framework/common/ClassGenerators.hpp"

namespace NCommand
{

class CSettings
{
ADD_ATTRIBUTE(CSettings, RevCtrlLogic, bool)
};

/**
 * Interface of terminal command
 */
class ITerminalCommand : public ITerminalSignals
{
    Q_OBJECT
public: // methods
    virtual ~ITerminalCommand() {}

public slots:
    virtual void run() = 0;
    virtual void appendData(const QString& str) = 0;
    virtual void terminate() = 0;
    virtual void setArgs(const QStringList& args) = 0;

    virtual void setWorkingDirectory(const QString& dir) = 0;
    virtual void setWorkingTime(uint32_t time) = 0;

    virtual CSettings getSettings() = 0;

signals:
    void started();
    void finished(int code);
    void log(const QString& str);
    void error(const QString& str);
    void logHtml(const QString& str);

protected: // methods
    virtual QString getManualMessage() = 0;
};

} // namespace NCommand

