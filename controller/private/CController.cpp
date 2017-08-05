/**
 * Project   AlgoVi
 *
 * @file     CController.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include <QDebug>

#include "algovi/system/CAlgoViSystem.hpp"
#include "controller/CController.hpp"
#include "controller/terminal/ITerminal.hpp"
#include "framework/settings/readers/CCommandConfigReader.hpp"
#include "framework/settings/readers/CRunnerConfigReader.hpp"
#include "framework/settings/readers/CTerminalConfigReader.hpp"
#include "gui/CMainWindow.hpp"

#include "view/CGraphicsSceneBase.hpp"
#include "view/CGraphicView.hpp"

namespace NController
{

CController::CController()
{
   NView::CGraphicsSceneBase* scene = new NView::CGraphicsSceneBase();
   NView::CGraphicView* view = new NView::CGraphicView(scene);
   view->show();
   if(readConfigs())
   {
      mGUIPtr.reset(new NMainWindow::CMainWindow());
      mSystemPtr.reset(new NAlgoVi::CAlgoViSystem(this));

      QObject::connect(mSystemPtr.get(), SIGNAL(log(QString)), mGUIPtr->getTerminal(), SLOT(appendSimpleText(QString)), Qt::QueuedConnection);
      QObject::connect(mGUIPtr->getTerminal(), &ITerminal::command, [this](const QString& cmd)
      {
         qDebug () << "CController:: new command: " << cmd;
         mGUIPtr->getTerminal()->setInsideProcessMode();
         mSystemPtr->executeCommand(cmd);
      });
      QObject::connect(mGUIPtr->getTerminal(), SIGNAL(newData(QString)), mSystemPtr.get(), SLOT(appendData(QString)), Qt::QueuedConnection);
      QObject::connect(mSystemPtr.get(), SIGNAL(finishedCommand()), mGUIPtr->getTerminal(), SLOT(unlock()), Qt::QueuedConnection);
      QObject::connect(mGUIPtr->getTerminal(), SIGNAL(termination()), mSystemPtr.get(), SLOT(terminateJob()));

      mGUIPtr->show();
   }
   else
   {
      std::exit(1);
   }
}

void CController::setComplation(std::shared_ptr<NCommand::CComplationProvider> provider)
{
    mGUIPtr->getTerminal()->setComplation(provider);
}

void CController::handleLog(const QString &text)
{
   mGUIPtr->getTerminal()->appendSimpleText(text);
}

void CController::handleLogHtml(const QString &text)
{
   mGUIPtr->getTerminal()->appendHtmlText(text);
}

void CController::handleError(const QString &text)
{
   mGUIPtr->getTerminal()->appendErrorText(text);
}

void CController::exit()
{
   mGUIPtr->close();
}

void CController::setInsideAppMode()
{
   mGUIPtr->getTerminal()->setInsideProcessMode();
}

void CController::lockTerminal()
{
   mGUIPtr->getTerminal()->lock();
}

void CController::unlockTerminal()
{
   mGUIPtr->getTerminal()->unlock();
}

void CController::setTerminalRevCtrlLogic()
{
   qDebug() << "set terminal rev ctrl logic";
   mGUIPtr->getTerminal()->setCommandSettings(CommandSettings().setCtrlLogic(true));
}

bool CController::readConfigs()
{
   try
   {
      NSettings::CCommandConfigReader cmdReader("configs/CommandConfig.xml");
      if(!cmdReader.fillSettings())
      {
         qDebug () << "Cannot fill command settings from config file";
         return false;
      }

      NSettings::CRunnerConfigReader runnerReader("configs/RunnerConfig.xml");
      if(!runnerReader.fillSettings())
      {
         qDebug () << "Cannot fill runner settings from config file";
         return false;
      }

      NSettings::CTerminalConfigReader terminalReader("configs/TerminalConfig.xml");
      if(!terminalReader.fillSettings())
      {
         qDebug () << "Cannot fill terminal settings from config file";
         return false;
      }
   }
   catch(const QString& msg)
   {
      qDebug () << msg;
      return false;
   }

   return true;
}

} // namespace NController
