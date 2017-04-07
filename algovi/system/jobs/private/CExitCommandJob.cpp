/**
 * Project   AlgoVi
 *
 * @file     CExitCommandJob.cpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#include "../CExitCommandJob.hpp"

namespace NAlgoVi
{

CExitCommandJob::CExitCommandJob(NController::CController *controller)
   : mControllerPtr(controller)
{

}

void CExitCommandJob::run(const QStringList &args)
{
   emit started();
   mControllerPtr->exit();
   emit finished();
}

void CExitCommandJob::appendData(const QString &data)
{

}

void CExitCommandJob::terminate()
{

}


} // namespace NAlgoVi
