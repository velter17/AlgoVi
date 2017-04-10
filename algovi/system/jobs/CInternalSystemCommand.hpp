/**
 * Project   AlgoVi
 *
 * @file     CInternalSystemCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#pragma once

#include <QSet>
#include <QString>

#include "algovi/system/jobs/IJob.hpp"
#include "controller/CController.hpp"

namespace NCommand
{
class CSystemCommand;
}

namespace NAlgoVi
{

class CInternalSystemCommand : public IJob
{
public: // methods
    CInternalSystemCommand(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;

public: // static methods
    static QStringList getCommandList();

private: // fields
    NCommand::CSystemCommand* mCommandPtr;
    NController::CController* mControllerPtr;
private: // static fields
    const static QStringList sAllowedCommands;
};

} // namespace NAlgoVi

