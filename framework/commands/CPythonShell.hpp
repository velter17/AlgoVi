/**
 * Project   AlgoVi
 *
 * @file     CPythonShell.hpp
 * @author   Dmytro Sadovyi
 * @date     07.04.2017
 */

#pragma once

#include "framework/commands/CSystemCommand.hpp"

namespace NCommand
{

class CPythonShell : public CSystemCommand
{
public: // methods
    void run() override;
};

} // namespace NCommand

