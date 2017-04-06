/**
 * Project   AlgoVi
 *
 * @file     CMainWindow.hpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#pragma once

#include <QMainWindow>

#include "controller/terminal/ITerminal.hpp"

namespace NMainWindow
{

class CMainWindow : public QMainWindow
{
public: // methods
    CMainWindow();
    ~CMainWindow();

    NController::ITerminal* getTerminal();

private: // fields
    QLayout* mMainLayout;
    NController::ITerminal* mTerminal;
};

} // namespace NMainWindow

