/**
 * Project   AlgoVi
 *
 * @file     CTesterJob.hpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"

namespace NController
{
class CController;
}

namespace NCommand
{
class CTesterCommand;
}

namespace NAlgoVi
{

class CTesterJob : public IJob
{
public: // methods
    CTesterJob(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;
    QStringList getArguments() override;
private: // fields
    NCommand::CTesterCommand* mProc;
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi
