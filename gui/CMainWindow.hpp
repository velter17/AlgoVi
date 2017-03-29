/**
 * Project   AlgoVi
 *
 * @file     CMainWindow.hpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#pragma once

#include <QMainWindow>

namespace NMainWindow
{

class CMainWindow : public QMainWindow
{
public: // methods
    CMainWindow();
    ~CMainWindow();

private: // fields
    QLayout* mMainLayout;
};

} // namespace NMainWindow

