/**
 * Project   AlgoVi
 *
 * @file     CApplicationRunner.hpp
 * @author   Dmytro Sadovyi
 * @date     17.05.2017
 */

#pragma once

#include "algovi/system/jobs/CJobBase.hpp"
#include "framework/commands/executor/CAppExecutor.hpp"

namespace NAlgoVi
{

class CApplicationRunner : public CJobBase <NCommand::CAppExecutor>
{
public: // methods
    CApplicationRunner(NController::CController* controller);
    void run(const QStringList& args) override;
};

} // namespace NAlgoVi

