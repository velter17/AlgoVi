/**
 * Project   AlgoVi
 *
 * @file     CController.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <memory>

#include "algovi/system/CAlgoViSystem.hpp"
#include "gui/CMainWindow.hpp"
#include "controller/terminal/ITerminal.hpp"

namespace NController
{

class CController
{
public: // methods
    CController();

private: // fields
    std::shared_ptr<NAlgoVi::CAlgoViSystem> mSystemPtr;
    std::shared_ptr<NMainWindow::CMainWindow> mGUIPtr;
};

} // namespace NController

