/**
 * Project   AlgoVi
 *
 * @file     CPythonInterpreter.cpp
 * @author   Dmytro Sadovyi
 * @date     09.04.2017
 */

#include <QDebug>

#include "../CPythonInterpreter.hpp"
#include "framework/commands/CPythonShell.hpp"

namespace NAlgoVi
{

CPythonInterpreter::CPythonInterpreter(NController::CController* controller)
    : mControllerPtr(controller)
{

}

void CPythonInterpreter::run(const QStringList& args)
{
    emit started();
    mPythonShell = new NCommand::CPythonShell();
    connect(mPythonShell, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
    connect(mPythonShell, SIGNAL(error(QString)), mControllerPtr, SLOT(handleLog(QString)));
    connect(mPythonShell, &NCommand::CPythonShell::finished, [this](int code)
    {
        qDebug () << "CPythonInterpreter finished with code " << code;
        emit finished();
        mPythonShell->deleteLater();
    });
    mPythonShell->run();
}

void CPythonInterpreter::appendData(const QString& data)
{
    mPythonShell->appendData(data);
}

void CPythonInterpreter::terminate()
{
    mPythonShell->terminate();
}


} // namespace NAlgoVi
