/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.hpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"
#include "framework/commands/ITerminalSignals.hpp"

namespace NController
{
class CController;
}

namespace NCommand
{
class CAppExecutor;
}

namespace NAlgoVi
{

class CAppExecutor : public IJob
{
public: // methods
    CAppExecutor(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;
    QStringList getArguments() override;

private: // fields
    NController::CController* mControllerPtr;
    NCommand::CAppExecutor* mExecutor;
};

} // namespace NAlgoVi

