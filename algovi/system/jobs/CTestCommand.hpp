/**
 * Project   AlgoVi
 *
 * @file     CTestCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     11.05.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"

namespace NController
{
class CController;
}

namespace NCommand
{
class CTestCommand;
}

namespace NAlgoVi
{

class CTestCommand : public IJob
{
public: // methods
    CTestCommand(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;
    QStringList getArguments() override;

private: // fields
    NCommand::CTestCommand* mProc;
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi

