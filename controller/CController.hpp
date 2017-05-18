/**
 * Project   AlgoVi
 *
 * @file     CController.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <QObject>
#include <memory>

#include "framework/complation/CComplationProvider.hpp"

namespace NAlgoVi
{
   class CAlgoViSystem;
}

namespace NMainWindow
{
   class CMainWindow;
}

namespace NController
{

class CController : public QObject
{
    Q_OBJECT
public: // methods
    CController();

    void setComplation(std::shared_ptr<NCommand::CComplationProvider> provider);

public slots:
    void handleLog(const QString& text);
    void handleLogHtml(const QString& text);
    void handleError(const QString& text);

    void exit();

    void setInsideAppMode();
    void lockTerminal();
    void unlockTerminal();
    void setTerminalRevCtrlLogic();

private: // methods
    bool readConfigs();

private: // fields
    std::shared_ptr<NAlgoVi::CAlgoViSystem> mSystemPtr;
    std::shared_ptr<NMainWindow::CMainWindow> mGUIPtr;
};

} // namespace NController

