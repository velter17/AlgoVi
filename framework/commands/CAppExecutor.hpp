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

#include "framework/commands/CCompiler.hpp"
#include "framework/commands/TProgLanguages.hpp"

namespace NCommand
{

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
signals:
    void finishedWithTime(uint32_t time);
private: // methods
    void compileCode(const QString& codePath,
                     const QStringList& flags,
                     const QStringList& args,
                     ProgLanguage::EType lang);
    void runApp(const QString& appPath, const QStringList& args);
    ProgLanguage::EType parseProgLanguage(
            boost::program_options::variables_map& varMap);
private: // fields
    QProcess* mProcess;
    std::vector <std::string> mParsedFlags;
    std::vector <std::string> mParsedArgs;
    QStringList mArgs;
    QMetaObject::Connection mErrorConnection;
    bool mForcedCompilation;
    std::string mInputFilePath;
    std::string mOutputFilePath;
    std::ofstream mOutputFile;
    int mTestToRun;
    OutputType::EType mOutputType;
    QString mOutputStorage;
    bool mDebugMode;
};

} // namespace NCommand

