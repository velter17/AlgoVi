/**
 * Project   AlgoVi
 *
 * @file     CJobForTest.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include "../CJobForTest.hpp"

namespace NAlgoVi
{

CJobForTest::CJobForTest(NController::CController *controller)
   : mControllerPtr(controller)
{
}

void CJobForTest::run(const QStringList &args)
{
   emit started();
}

void CJobForTest::appendData(const QString &data)
{
   if(data == "exit")
   {
      emit finished();
   }
   else
   {
      mControllerPtr->handleLog("CJobForTest::appendData received: " + data + "\n");
   }
}

void CJobForTest::terminate()
{
   emit finished();
}


} // namespace NAlgoVi
