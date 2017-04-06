/**
 * Project  AlgoVi
 *
 * @file    main.cpp
 * @author  Dmytro Sadovyi
 * @date    25.03.2017
*/

#include <QApplication>
#include <iostream>

#include "controller/CController.hpp"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    NController::CController controller;
    return app.exec();
}
