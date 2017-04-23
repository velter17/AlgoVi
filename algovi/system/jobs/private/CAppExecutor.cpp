/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.cpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#include <QDebug>

#include "../CAppExecutor.hpp"
#include "controller/CController.hpp"
#include "framework/commands/CAppExecutor.hpp"

namespace NAlgoVi
{

CAppExecutor::CAppExecutor(NController::CController* controller)
    : mControllerPtr(controller)
{

}

void CAppExecutor::run(const QStringList& args)
{
    emit started();
    mExecutor = new NCommand::CAppExecutor();
    connect(mExecutor, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
    connect(mExecutor, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
    connect(mExecutor, &NCommand::CAppExecutor::finished, [this](int code)
    {
        qDebug () << "NCommand::AppExecutor command finished with code " << code;
        emit finished();
        mExecutor->deleteLater();
    });
    mExecutor->setArgs(args);
    mExecutor->run();
}

void CAppExecutor::appendData(const QString& data)
{
    mExecutor->appendData(data);
}

void CAppExecutor::terminate()
{
    mExecutor->terminate();
}

} // namespace NAlgoVi
