/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.hpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#pragma once

#include <fstream>
#include <QProcess>
#include <QTimer>

#include "framework/commands/CCompiler.hpp"

namespace NCommand
{

struct ReturnCodes
{
   enum EType
   {
      Success,
      CompilationError,
      RuntimeError,
      TimeLimitExceeded,
   };
};

struct OutputType
{
   enum EType
   {
      NoSpecified,
      JustEmit,
      ToFile,
      ToStorage,
   };
};

class CAppExecutor : public CTerminalCommandBase
{
public: // methods
    CAppExecutor();
    ~CAppExecutor();

    void run() override;
    void appendData(const QString& str) override;
    void setArgs(const QStringList& args) override;
    void terminate() override;

    void setOutputType(OutputType::EType type);
    QString getOutput();

    std::uint32_t getExecutionTime();
signals:
    void finishedWithTime(uint32_t time);
private: // methods
    void compileCode(const QString& codePath,
                     const QStringList& flags,
                     const QStringList& args,
                     const QString& lang);
    void runApp(const QString& appPath, const QStringList& args);
    QString parseProgLanguage(
            boost::program_options::variables_map& varMap);
private: // fields
    QProcess* mProcess;
    std::vector <std::string> mParsedFlags;
    std::vector <std::string> mParsedArgs;
    QStringList mArgs;
    QMetaObject::Connection mErrorConnection;
    bool mForcedCompilation;

    int mTimeLimit;
    QTimer mTimer;
    int mExecutionTime;

    bool mTimeLimitExceeded;
    bool mCrashed;

    std::string mInputFilePath;
    std::string mOutputFilePath;
    std::ofstream mOutputFile;
    OutputType::EType mOutputType;
    QString mOutputStorage;

    int mTestToRun;
    bool mDebugMode;
    bool mRemoveDebug;
};

} // namespace NCommand

