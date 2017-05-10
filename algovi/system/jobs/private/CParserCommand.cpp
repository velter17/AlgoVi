/**
 * Project   AlgoVi
 *
 * @file     CParserCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     11.05.2017
 */

#include <QDebug>

#include "algovi/system/jobs/CParserCommand.hpp"
#include "controller/CController.hpp"
#include "framework/commands/parser/CParserCommand.hpp"

namespace NAlgoVi
{

CParserCommand::CParserCommand(NController::CController* controller)
   : mControllerPtr(controller)
{
   mParser = new NCommand::CParserCommand();
}

void CParserCommand::run(const QStringList &args)
{
   emit started();
   connect(mParser, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
   connect(mParser, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
   connect(mParser, &NCommand::CParserCommand::finished, [this](int code)
   {
       qDebug () << "NCommand::CParserCommand command finished with code " << code;
       emit finished();
       mParser->deleteLater();
   });
   mParser->setArgs(args);
   mParser->run();
}

void CParserCommand::appendData(const QString &data)
{
   mParser->appendData(data);
}

void CParserCommand::terminate()
{
   mParser->terminate();
}

QStringList CParserCommand::getArguments()
{
   return mParser->getOptionsList();
}


} // namespace NAlgoVi
