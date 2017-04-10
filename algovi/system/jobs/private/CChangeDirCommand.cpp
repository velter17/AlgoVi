/**
 * Project   AlgoVi
 *
 * @file     CChangeDirCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     11.04.2017
 */

#include <QDebug>

#include "../CChangeDirCommand.hpp"
#include "controller/CController.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NAlgoVi
{

CChangeDirCommand::CChangeDirCommand(NController::CController* controller)
    : mControllerPtr(controller)
{

}

void CChangeDirCommand::run(const QStringList& args)
{
    emit started();
    if(args.size() != 2)
    {
        mControllerPtr->handleError(" [ Error ] wrong args for command 'cd'\n");
        emit finished();
        return;
    }
    qDebug() << "args are " << args;
    QString dir = *(args.begin() + 1);
    if(!NFileSystem::change_dir(dir))
    {
        mControllerPtr->handleError(" [ Error ] " + dir + " - directory doesn't exist\n");
    }
    emit finished();
}

void CChangeDirCommand::appendData(const QString& data)
{

}

void CChangeDirCommand::terminate()
{

}


} // namespace NAlgoVi
