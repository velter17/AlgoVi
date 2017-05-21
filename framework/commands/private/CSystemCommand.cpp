/**
 * Project   AlgoVi
 *
 * @file     CSystemCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#include <QDebug>
#include <QTimer>

#include "../CSystemCommand.hpp"
#include "framework/commands/ProcessHelper.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

CSystemCommand::CSystemCommand()
{

}

void CSystemCommand::run()
{
   emit started();
   if(mArgs.empty())
   {
      emit error("No args for system command\n");
      emit finished(-1);
      return;
   }

   mProc = new QProcess();
   connect(mProc, &QProcess::readyReadStandardOutput, [this](){
      emit log(mProc->readAllStandardOutput());
   });
   connect(mProc, &QProcess::readyReadStandardError, [this](){
      emit error(mProc->readAllStandardError());
   });
   mProc->setWorkingDirectory(mWorkingDirectory);

   if(mWorkingTime != 0)
   {
      QTimer* timer = new QTimer();
      timer->setInterval(mWorkingTime);
      connect(timer, SIGNAL(timeout()), mProc, SLOT(kill()));
      connect(mProc, SIGNAL(started()), timer, SLOT(start()));
      connect(mProc, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
              [this, timer](int code)
      {
         timer->deleteLater();
      });
   }
   connect(mProc, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
           [this](int code)
   {
      emit finished(code);
      mProc->deleteLater();
   });

   connect(mProc, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
                                  [this](QProcess::ProcessError err){
           qDebug () << "CSystemCommand: mProc emitted error" << processErrorToStr(err);
           if(err == QProcess::FailedToStart)
           {
               emit error(processErrorToStr(err) + "\n");
               mProc->deleteLater();
               emit finished(1);
            }
       });

#ifdef __linux__
   QString appList;
   for(const QString& str : mArgs)
   {
       appList += " " + str;
   }
   mProc->start("bash", QStringList() << "-c" << appList);
   qDebug () << appList << " have started...";
#else
   QString app = *mArgs.begin();
   if(app == "cmd.exe")
   {
       mArgs = mArgs.mid(1);
   }
   else
   {
       mArgs.erase(mArgs.begin());
       app = NFileSystem::get_full_system_path(app);
   }
   mProc->start(app, mArgs);
   qDebug () << app << " " << mArgs << " have started...";
#endif
}

void CSystemCommand::appendData(const QString &str)
{
    mProc->write(str.toLocal8Bit());
    mProc->write("\n");
}

void CSystemCommand::setArgs(const QStringList& args)
{
   mArgs = args;
}

void CSystemCommand::terminate()
{
    mProc->terminate();
}


} // namespace NCommand
