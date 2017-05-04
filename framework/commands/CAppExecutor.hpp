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

class CAppExecutor : public CTerminalCommandBase
{
public: // methods
    CAppExecutor();

    void run() override;
    void appendData(const QString& str) override;
    void setArgs(const QStringList& args) override;
    void terminate() override;

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
};

} // namespace NCommand

