/**
 * Project   AlgoVi
 *
 * @file     CController.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include <QDebug>

#include "algovi/system/CAlgoViSystem.hpp"
#include "controller/terminal/ITerminal.hpp"
#include "gui/CMainWindow.hpp"
#include "../CController.hpp"

namespace NController
{

CController::CController()
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

} // namespace NController
