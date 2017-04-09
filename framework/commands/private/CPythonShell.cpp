/**
 * Project   AlgoVi
 *
 * @file     CPythonShell.cpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#include "../CPythonShell.hpp"

namespace NCommand
{

void CPythonShell::run()
{
    setArgs(QStringList() << "/usr/bin/python -i");
    CSystemCommand::run();
}


} // namespace NCommand
