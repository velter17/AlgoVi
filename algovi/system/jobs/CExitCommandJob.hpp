/**
 * Project   AlgoVi
 *
 * @file     CExitCommandJob.hpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"
#include "controller/CController.hpp"

namespace NAlgoVi
{

class CExitCommandJob : public IJob
{
public: // methods
    CExitCommandJob(NController::CController* controller);

    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;

private: // fields
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi

