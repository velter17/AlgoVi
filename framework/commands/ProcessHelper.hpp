/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.cpp
 * @author   Dmytro Sadovyi
 * @date     11.05.2017
 */

#pragma once

#include <QProcess>
#include <QString>

namespace NCommand
{

inline QString processErrorToStr(QProcess::ProcessError err)
{
    if(err == QProcess::ProcessError::FailedToStart)
    {
        return "Failed to start";
    }
    else if(err == QProcess::ProcessError::Crashed)
    {
        return "Crashed";
    }
    else if(err == QProcess::ProcessError::Timedout)
    {
        return "Timedout";
    }
    else if(err == QProcess::ProcessError::ReadError)
    {
        return "Read error";
    }
    else if(err == QProcess::ProcessError::WriteError)
    {
        return "Write error";
    }
    else
    {
        return "Unknown error";
    }
}

} // namespace NCommand
