/**
 * Project   AlgoVi
 *
 * @file     CAlgoViSystem.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include <QDebug>
#include <cassert>

#include "controller/CController.hpp"
#include "../CAlgoViSystem.hpp"

namespace NAlgoVi
{

CAlgoViSystem::CAlgoViSystem(NController::CController* controller)
   : mControllerPtr(controller)
   , mCommandHandler(controller)
{
}

void CAlgoViSystem::executeCommand(const QString& command)
{
   QStringList args = command.split(" ");
   mJobPtr = mCommandHandler.getJob(*args.begin());
   if(0 == mJobPtr)
   {
      mControllerPtr->handleError("[ Error ] unknown command \'" + (*args.begin()) + "\'");
      emit finishedCommand();
      return;
   }
   connect(mJobPtr.get(), &IJob::started, [this](){
      mControllerPtr->handleLog("[job started]\n");
   });
   connect(mJobPtr.get(), &IJob::finished, [this](){
      mControllerPtr->handleLog("[job finished]\n");
      emit finishedCommand();
   });
   //mControllerPtr->setInsideAppMode();
   mJobPtr->run(args);
}

void CAlgoViSystem::appendData(const QString &data)
{
   assert(mJobPtr != 0);

   mJobPtr->appendData(data);
}


} // namespace NAlgoVi
