/**
 * Project  AlgoVi
 *
 * @file    main.cpp
 * @author  Dmytro Sadovyi
 * @date    25.03.2017
*/

#include <QApplication>
#include <iostream>

#include "gui/CMainWindow.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    NMainWindow::CMainWindow win;
    win.show();
    return app.exec();
}
