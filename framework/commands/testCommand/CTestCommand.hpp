/**
 * Project   AlgoVi
 *
 * @file     CTestCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     09.05.2017
 */

#pragma once

#include "framework/commands/CTerminalCommandBase.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"

namespace NCommand
{

class CTestCommand : public CTerminalCommandBase
{
public: // methods
    CTestCommand();
    void run() override;
    void setArgs(const QStringList& args) override;

private: // methods
    void print();
    void printTests(const tRange& range);
    void printTests(const tList& list);
    void printTest(int idx);

private: // fields
    QStringList mArgs;
    boost::program_options::variables_map mVarMap;
};

} // namespace NCommand

