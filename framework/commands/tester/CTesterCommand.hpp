/**
 * Project   AlgoVi
 *
 * @file     CTesterCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#pragma once

#include <functional>

#include "framework/commands/CCompiler.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"

namespace NCommand
{

class CTesterImpl;

class CTesterCommand : public CTerminalCommandBase
{
   Q_OBJECT
public: // methods
    CTesterCommand();
    void run() override;
    void appendData(const QString& str) override;
    void setArgs(const QStringList& args) override;
    void terminate() override;
private: // methods
    void compile();

    void testRange(tRange range, int test);
    void testList(const tList& list, int idx);
    void runTest(int idx, std::function<void()> callback);

    void finish();
signals:
    void compilationFinished(int code);
private: // fields
    QStringList mArgs;
    QString mCodePath;
    QString mCheckerPath;
    QString mAppPath;
    QString mCheckerAppPath;
    bool mVerboseFlag;
    CTesterImpl* mTesterImpl;
    int mAcceptedTests;
    int mCheckedTests;
    bool mCustomChecker;
    int mTimeLimit;
    int mInfoMask;
    bool mTerminatedFlag;
};

} // namespace NCommand

