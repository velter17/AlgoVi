/**
 * Project   AlgoVi
 *
 * @file     CTestReader.hpp
 * @author   Dmytro Sadovyi
 * @date     13.05.2017
 */

#pragma once

#include <mutex>
#include <thread>

#include "framework/commands/CTerminalCommandBase.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"

namespace NCommand
{

class CTestReader : public CTerminalCommandBase
{
public: // methods
    CTestReader();
    void run() override;
    void terminate() override;
    void setArgs(const QStringList& args) override;

private: // methods
    tTest readTest(int idx);
    void readTestsRange(tRange range);
    void readTestsList(const tList& list);
private: // fields
    QStringList mArgs;
    bool mTerminatedFlag;
    QString mInputPattern;
    QString mOutputPattern;
    QString mFolder;
    int mNumLength;
    int mTestsRead;
    std::mutex mDataAccess;
};

} // namespace NCommand

