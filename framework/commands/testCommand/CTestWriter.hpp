/**
 * Project   AlgoVi
 *
 * @file     CTestWriter.hpp
 * @author   Dmytro Sadovyi
 * @date     14.05.2017
 */

#pragma once

#include <mutex>
#include <thread>

#include "framework/commands/CTerminalCommandBase.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"

namespace NCommand
{

class CTestWriter : public CTerminalCommandBase
{
public: // methods
    CTestWriter();
    void run() override;
    void terminate() override;
    void setArgs(const QStringList& args) override;

private: // methods
    void writeTest(int idx);
    void writeTestsRange(tRange range);
    void writeTestsList(const tList& list);
private: // fields
    QStringList mArgs;
    bool mTerminatedFlag;
    QString mInputPattern;
    QString mOutputPattern;
    QString mFolder;
    int mNumLength;
    int mStartFrom;
    int mTestsWritten;
    std::mutex mDataAccess;
};

} // namespace NCommand

