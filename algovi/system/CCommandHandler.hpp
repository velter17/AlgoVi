/**
 * Project   AlgoVi
 *
 * @file     CCommandHandler.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <memory>

#include "algovi/system/jobs/IJob.hpp"

namespace NController
{
   class CController;
}

namespace NAlgoVi
{

class CCommandHandler
{
public: // methods
    CCommandHandler(NController::CController* controller);

    std::shared_ptr <IJob> getJob(const QString& cmd);

private: // fields
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi

