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
#include "framework/settings/CCommandSettings.hpp"

namespace NAlgoVi
{

CInternalSystemCommand::CInternalSystemCommand(NController::CController* controller)
    : mControllerPtr(controller)
{

}

void CInternalSystemCommand::run(const QStringList& args)
{
    emit started();
    if(args.isEmpty() || NSettings::CCommandSettings::getInstance()
            .getCommands().find(*args.begin()) ==
            NSettings::CCommandSettings::getInstance().getCommands().end())
    {
        mControllerPtr->handleError(" [ Error ] no cmd str for command " + (*args.begin()) +
                                    " [internal error]\n");
        emit finished();
    }
    mCommandPtr = new NCommand::CSystemCommand();
    connect(mCommandPtr, SIGNAL(log(QString)), mControllerPtr, SLOT(handleLog(QString)));
    connect(mCommandPtr, SIGNAL(error(QString)), mControllerPtr, SLOT(handleError(QString)));
    connect(mCommandPtr, &NCommand::CSystemCommand::finished, [this](int code)
    {
        qDebug () << "System command finished with code " << code;
        emit finished();
        mCommandPtr->deleteLater();
    });
    QString propCommand = NSettings::CCommandSettings::getInstance().getCommands()[*args.begin()];
    mCommandPtr->setArgs(QStringList() << propCommand);
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

} // namespace NAlgoVi
