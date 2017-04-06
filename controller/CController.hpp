/**
 * Project   AlgoVi
 *
 * @file     CController.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <memory>

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

class CController
{
public: // methods
    CController();

public slots:
    void handleLog(const QString& text);
    void handleLogHtml(const QString& text);
    void handleError(const QString& text);

    void setInsideAppMode();
    void lockTerminal();
    void unlockTerminal();

private: // fields
    std::shared_ptr<NAlgoVi::CAlgoViSystem> mSystemPtr;
    std::shared_ptr<NMainWindow::CMainWindow> mGUIPtr;
};

} // namespace NController

