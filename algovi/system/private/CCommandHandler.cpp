/**
 * Project   AlgoVi
 *
 * @file     CCommandHandler.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include "algovi/system/jobs/CEmptyJob.hpp"
#include "algovi/system/jobs/CJobForTest.hpp"
#include "controller/CController.hpp"
#include "../CCommandHandler.hpp"

namespace NAlgoVi
{

CCommandHandler::CCommandHandler(NController::CController *controller)
   : mControllerPtr(controller)
{

}

std::shared_ptr<IJob> CCommandHandler::getJob(const QString& cmd)
{
   if(cmd == "test_command")
      return std::make_shared<CJobForTest>(mControllerPtr);
   if(cmd.isEmpty())
      return std::make_shared<CEmptyJob>();
   else
      return std::shared_ptr<IJob>();
}


} // namespace NAlgoVi
