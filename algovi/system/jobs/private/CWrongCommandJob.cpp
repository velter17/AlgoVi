/**
 * Project   AlgoVi
 *
 * @file     CWrongCommandJob.cpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#include <QStringList>

#include "../CWrongCommandJob.hpp"

namespace NAlgoVi
{

CWrongCommandJob::CWrongCommandJob(NController::CController *controller)
   : mControllerPtr(controller)
{

}

void CWrongCommandJob::run(const QStringList& args)
{
   emit started();
   QString command_name = *args.begin();
   mControllerPtr->handleError(" [ Error ] " + command_name + " - command not found\n");
   emit finished();
}

void CWrongCommandJob::appendData(const QString& data)
{

}

void CWrongCommandJob::terminate()
{

}


} // namespace NAlgoVi
