/**
 * Project   AlgoVi
 *
 * @file     CTesterImpl.hpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#pragma once

#include <QObject>
#include <QTemporaryDir>

#include "framework/commands/CAppExecutor.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/common/ClassGenerators.hpp"

namespace NCommand
{

struct TesterResult
{
   enum EType
   {
      Accepted,
      WrongAnswer,
      TimeLimitExceeded,
      RuntimeError,
      CheckFailed,
      PresentationError,

      Total
   };
};

class CTesterJob
{
ADD_ATTRIBUTE(CTesterJob, AppPath, QString)
ADD_ATTRIBUTE(CTesterJob, CheckerPath, QString)
ADD_ATTRIBUTE(CTesterJob, TimeLimit, uint32_t)
ADD_ATTRIBUTE(CTesterJob, TestNumber, uint32_t)
ADD_ATTRIBUTE(CTesterJob, Verbose, bool)
};

class CTesterResult
{
ADD_ATTRIBUTE(CTesterResult, Result, TesterResult::EType)
ADD_ATTRIBUTE(CTesterResult, Message, QString)
ADD_ATTRIBUTE(CTesterResult, ExecutionTime, uint32_t)
ADD_ATTRIBUTE(CTesterResult, Output, QString)
};

class CTesterImpl : public QObject
{
   Q_OBJECT
public: // methods
    CTesterImpl(const CTesterJob& job);
    void execute();
signals:
    void finished(const CTesterResult& result);
private: // methods
    void check(const tTest& test);
private: // fields
    CTesterJob mJob;
    CAppExecutor* mExecutor;
    QTemporaryDir mTempDir;
    QString mOutput;
};

} // namespace NCommand

