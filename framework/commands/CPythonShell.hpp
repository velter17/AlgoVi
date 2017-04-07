/**
 * Project   AlgoVi
 *
 * @file     CPythonShell.hpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#pragma once

#include "framework/commands/CTerminalCommandBase.hpp"

namespace NCommand
{

class CPythonShell : public CTerminalCommandBase
{
public: // methods
    CPythonShell();

    void run() override;
};

} // namespace NCommand

