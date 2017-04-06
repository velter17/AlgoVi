/**
 * Project   AlgoVi
 *
 * @file     CJobForTest.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"
#include "controller/CController.hpp"
#include "framework/commands/ITerminalSignals.hpp"

namespace NAlgoVi
{

class CJobForTest : public IJob
{
public: // methods
    CJobForTest(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;

private: // fields
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi

