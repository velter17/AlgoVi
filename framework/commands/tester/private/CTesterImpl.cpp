/**
 * Project   AlgoVi
 *
 * @file     CTesterImpl.cpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#include <QDebug>
#include <QMap>

#include "framework/commands/tester/private/CTesterImpl.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

TesterResult::EType codeToTesterResult(int code)
{
   static QMap <ReturnCodes::EType, TesterResult::EType> mp {
      {ReturnCodes::Success, TesterResult::Accepted},
      {ReturnCodes::CompilationError, TesterResult::RuntimeError},
      {ReturnCodes::TimeLimitExceeded, TesterResult::TimeLimitExceeded},
      {ReturnCodes::RuntimeError, TesterResult::RuntimeError},
   };
   return mp[static_cast<ReturnCodes::EType>(code)];
}

CTesterImpl::CTesterImpl(const CTesterJob& job)
   : mJob(job)
{
    mTempDir.setAutoRemove(true);
}

void CTesterImpl::execute()
{
   mExecutor = new CAppExecutor();
   mExecutor->setArgs(QStringList()
                      << "--src" << mJob.getAppPath()
                      << "--test" << QString::number(mJob.getTestNumber())
                      << "--time-limit" << QString::number(mJob.getTimeLimit()));

   mExecutor->setOutputType(OutputType::ToStorage);

   connect(mExecutor, &CAppExecutor::finished, [this](int code){
       qDebug () << "CTesterImpl::execute mExecutor finished with code " << code;
       int execTime = mExecutor->getExecutionTime();
       if(code == 0)
       {
           mOutput = mExecutor->getOutput();
           while(mOutput.endsWith('\n'))
           {
              mOutput.chop(1);
           }
           mExecutor->deleteLater();
           check(CTestProvider::getInstance().getTest(mJob.getTestNumber()-1), execTime);
       }
       else
       {
           mExecutor->deleteLater();
           emit finished(CTesterResult().setResult(codeToTesterResult(code))
                                        .setMessage("returned with code " + QString::number(code))
                                        .setExecutionTime(execTime));
       }
   });

   mExecutor->run();
}

void CTesterImpl::terminate()
{
   mExecutor->terminate();
}

void CTesterImpl::check(const tTest& test, int execTime)
{
    qDebug () << "check function started";
    std::vector <QPair<QString, const QString&>> filesVec {
        {mTempDir.path() + "/input.txt", test.first},
        {mTempDir.path() + "/output.txt", mOutput},
        {mTempDir.path() + "/answer.txt", test.second},
        {mTempDir.path() + "/result.txt", mOutput},
    };

    for(std::size_t i = 0; i < 3; ++i)
    {
        auto& itr = filesVec[i];
        QFile file(itr.first);
        file.open(QFile::WriteOnly);
        file.write(itr.second.toLocal8Bit());
        file.close();
    }

    mExecutor = new CAppExecutor();
    QStringList args;
    args << "--src" << mJob.getCheckerPath();
    for(auto& itr : filesVec)
    {
        args << "--arg" << itr.first;
    }
    if(mJob.getCustomChecker())
    {
        args << "--output" << filesVec.back().first;
    }
    mExecutor->setArgs(args);
    connect(mExecutor, &CAppExecutor::finished, [this, filesVec, execTime](int code){
        qDebug () << "checker finished with code " << code;
        mExecutor->deleteLater();
        TesterResult::EType result;
        if(code == 0)
        {
            result = TesterResult::Accepted;
        }
        else
        {
            result = TesterResult::WrongAnswer;
        }
        QFile report(filesVec.back().first);
        if(!report.open(QFile::ReadOnly))
        {
            emit finished(CTesterResult().setResult(TesterResult::CheckFailed)
                                         .setMessage("cannot open checker-result file")
                                         .setExecutionTime(execTime));
        }
        else
        {
            QString data = report.readAll();
            while(data.endsWith('\n'))
            {
               data.chop(1);
            }
            emit finished(CTesterResult().setResult(result)
                                         .setMessage(data)
                                         .setExecutionTime(execTime)
                                         .setOutput(mJob.getVerbose() ? mOutput : ""));
        }
    });

    mExecutor->run();
}

} // namespace NCommand
