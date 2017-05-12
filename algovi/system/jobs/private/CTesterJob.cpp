/**
 * Project   AlgoVi
 *
 * @file     CTesterJob.cpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#include <QDebug>

#include "algovi/system/jobs/CTesterJob.hpp"
#include "controller/CController.hpp"
#include "framework/commands/tester/CTesterCommand.hpp"
namespace NAlgoVi
{

CTesterJob::CTesterJob(NController::CController* controller)
   : mControllerPtr(controller)
{
   mProc = new NCommand::CTesterCommand();
}

void CTesterJob::run(const QStringList& args)
{
   emit started();
   connect(mProc, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
   connect(mProc, SIGNAL(logHtml(QString)), mControllerPtr, SLOT(handleLogHtml(QString)));
   connect(mProc, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
   connect(mProc, &NCommand::CTesterCommand::finished, [this](int code)
   {
       qDebug () << "NCommand::CTesterJob command finished with code " << code;
       emit finished();
       mProc->deleteLater();
   });
   mProc->setArgs(args);
   mProc->run();
}

void CTesterJob::appendData(const QString &data)
{
   mProc->appendData(data);
}

void CTesterJob::terminate()
{
   mProc->terminate();
}

QStringList CTesterJob::getArguments()
{
   return mProc->getOptionsList();
}

} // namespace NAlgoVi
