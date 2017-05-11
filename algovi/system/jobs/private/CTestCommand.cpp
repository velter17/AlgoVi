/**
 * Project   AlgoVi
 *
 * @file     CTestCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     11.05.2017
 */

#include <QDebug>

#include "../CTestCommand.hpp"
#include "controller/CController.hpp"
#include "framework/commands/testCommand/CTestCommand.hpp"

namespace NAlgoVi
{

CTestCommand::CTestCommand(NController::CController* controller)
   : mControllerPtr(controller)
{
   mProc = new NCommand::CTestCommand();
}

void CTestCommand::run(const QStringList& args)
{
   emit started();
   connect(mProc, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
   connect(mProc, SIGNAL(logHtml(QString)), mControllerPtr, SLOT(handleLogHtml(QString)));
   connect(mProc, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
   connect(mProc, &NCommand::CTestCommand::finished, [this](int code)
   {
       qDebug () << "NCommand::CTestCommand command finished with code " << code;
       emit finished();
       mProc->deleteLater();
   });
   mProc->setArgs(args);
   mProc->run();
}

void CTestCommand::appendData(const QString &data)
{

}

void CTestCommand::terminate()
{

}

QStringList CTestCommand::getArguments()
{
   return mProc->getOptionsList();
}

} // namespace NAlgoVi
