/**
 * Project   AlgoVi
 *
 * @file     CInternalSystemCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#include <QDebug>

#include "../CInternalSystemCommand.hpp"
#include "framework/commands/CSystemCommand.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NAlgoVi
{

const QStringList CInternalSystemCommand::sAllowedCommands = {
    "ls", "mkdir", "rm", "pwd", "cat", "echo", "printf", "date", "time"
};

CInternalSystemCommand::CInternalSystemCommand(NController::CController* controller)
    : mControllerPtr(controller)
{

}

void CInternalSystemCommand::run(const QStringList& args)
{
    emit started();
    mCommandPtr = new NCommand::CSystemCommand();
    connect(mCommandPtr, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
    connect(mCommandPtr, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
    connect(mCommandPtr, &NCommand::CSystemCommand::finished, [this](int code)
    {
        qDebug () << "System command finished with code " << code;
        emit finished();
        mCommandPtr->deleteLater();
    });
    mCommandPtr->setArgs(args);
    mCommandPtr->setWorkingDirectory(NFileSystem::get_current_dir());
    mCommandPtr->run();
}

void CInternalSystemCommand::appendData(const QString& data)
{
    mCommandPtr->appendData(data);
}

void CInternalSystemCommand::terminate()
{
    mCommandPtr->terminate();
}

QStringList CInternalSystemCommand::getCommandList()
{
    return sAllowedCommands;
}

} // namespace NAlgoVi
