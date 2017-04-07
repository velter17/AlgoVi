/**
 * Project   AlgoVi
 *
 * @file     CSystemCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#pragma once

#include <QProcess>

#include "framework/commands/CTerminalCommandBase.hpp"

namespace NCommand
{

class CSystemCommand : public CTerminalCommandBase
{
public: // methods
    CSystemCommand();

    void run() override;
    void setArgs(const QStringList& args) override;
    void terminate() override;

private: // fields
    QStringList mArgs;
    QProcess* mProc;
};

} // namespace NCommand

