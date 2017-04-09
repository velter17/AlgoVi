/**
 * Project   AlgoVi
 *
 * @file     CPythonInterpreter.hpp
 * @author   Dmytro Sadovyi
 * @date     09.04.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"
#include "controller/CController.hpp"

namespace NCommand
{
class CPythonShell;
}

namespace NAlgoVi
{

class CPythonInterpreter : public IJob
{
public: // methods
    CPythonInterpreter(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;

private: // fields
    NController::CController* mControllerPtr;
    NCommand::CPythonShell* mPythonShell;
};

} // namespace NAlgoVi

