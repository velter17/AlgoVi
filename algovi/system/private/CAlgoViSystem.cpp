/**
 * Project   AlgoVi
 *
 * @file     CAlgoViSystem.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include <QDebug>

#include "../CAlgoViSystem.hpp"

namespace NAlgoVi
{

CAlgoViSystem::CAlgoViSystem()
{
}

void CAlgoViSystem::executeCommand(const QString& command)
{
   qDebug() << "CAlgoViSystem::executeCommand() " << command;
   emit log("CAlgoViSystem handled " + command);
   emit finishedCommand();
}


} // namespace NAlgoVi
