/**
 * Project   AlgoVi
 *
 * @file     CChangeDirCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     11.04.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"

namespace NController
{
class CController;
}

namespace NAlgoVi
{

class CChangeDirCommand : public IJob
{
public: // methods
    CChangeDirCommand(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;

private: // fields
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi

