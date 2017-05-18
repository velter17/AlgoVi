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

namespace NAlgoViGUI
{
class CTestEditor;
}

namespace NCommand
{

class CTestCreator;

class CTestCommand : public CTerminalCommandBase
{
public: // methods
    CTestCommand();
    void run() override;
    void appendData(const QString& str) override;
    void terminate() override;
    void setArgs(const QStringList& args) override;

private: // methods
    void print();
    void printTests(const tRange& range);
    void printTests(const tList& list);
    void printTest(int idx);

    void deleteTests();
    void createTest();
    void swapTests();
    void moveTest();
    void editTest();

private: // fields
    QStringList mArgs;
    boost::program_options::variables_map mVarMap;
    std::shared_ptr <CTestCreator> mTestCreator;
    std::shared_ptr <NAlgoViGUI::CTestEditor> mTestEditor;
};

} // namespace NCommand

