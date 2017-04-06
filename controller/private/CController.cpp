/**
 * Project   AlgoVi
 *
 * @file     CController.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include <QDebug>

#include "../CController.hpp"

namespace NController
{

CController::CController()
{
   mSystemPtr.reset(new NAlgoVi::CAlgoViSystem());
   mGUIPtr.reset(new NMainWindow::CMainWindow());

   QObject::connect(mSystemPtr.get(), SIGNAL(log(QString)), mGUIPtr->getTerminal(), SLOT(appendSimpleText(QString)), Qt::QueuedConnection);
   QObject::connect(mGUIPtr->getTerminal(), &ITerminal::command, [this](const QString& cmd)
   {
      qDebug () << "CController:: new command: " << cmd;
      mGUIPtr->getTerminal()->setInsideProcessMode();
      mSystemPtr->executeCommand(cmd);
   });
   QObject::connect(mSystemPtr.get(), SIGNAL(finishedCommand()), mGUIPtr->getTerminal(), SLOT(unlock()), Qt::QueuedConnection);

   mGUIPtr->show();
}

} // namespace NController
