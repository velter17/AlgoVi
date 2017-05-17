/**
 * Project   AlgoVi
 *
 * @file     CApplicationRunner.cpp
 * @author   Dmytro Sadovyi
 * @date     17.05.2017
 */

#include <QDebug>

#include "algovi/system/jobs/CApplicationRunner.hpp"

namespace NAlgoVi
{

CApplicationRunner::CApplicationRunner(NController::CController* controller)
   : CJobBase<NCommand::CAppExecutor>(controller)
{

}

void CApplicationRunner::run(const QStringList &args)
{
   emit started();
   connect(mProc, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
   connect(mProc, SIGNAL(logHtml(QString)), mControllerPtr, SLOT(handleLogHtml(QString)));
   connect(mProc, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
   connect(mProc, &NCommand::CAppExecutor::finished, [this](int code)
   {
       qDebug () << "executor was finished with code " << code;
       if(code == NCommand::ReturnCodes::TimeLimitExceeded)
       {
          mControllerPtr->handleLog(" [ Info ] time limit exceeded\n");
       }
       else if(code == NCommand::ReturnCodes::RuntimeError)
       {
          mControllerPtr->handleError(" [ Error ] Process was crashed\n");
       }
       else
       {
          mControllerPtr->handleLog(" [ Info ] time: " + QString::number(mProc->getExecutionTime()) + "\n");
       }
       mProc->deleteLater();
       emit finished();
   });
   mProc->setArgs(args);
   mProc->run();
}


} // namespace NAlgoVi
