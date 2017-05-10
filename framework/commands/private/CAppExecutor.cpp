/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.cpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#include <QDebug>

#include "framework/commands/CAppExecutor.hpp"
#include "framework/commands/CCompiler.hpp"
#include "framework/filesystem/filesystem.hpp"
#include "framework/commands/ProcessHelper.hpp"

namespace NCommand
{

CAppExecutor::CAppExecutor()
{
    mOptions.add_options()
        ("src,s", boost::program_options::value <std::string>()->required(),
            "source file for execution")
        ("lang,l", boost::program_options::value <std::string>(),
            "language of source file\n"
            "if not specified - recognized by extension\n"
            "if no extension - just execute the file")
        ("flag,f", boost::program_options::value <std::vector<std::string>>(&mParsedFlags),
            "flag for compilation ('-f val' will be '-val'")
        ("force", boost::program_options::bool_switch(),
            "forced compilation")
        ("arg,a", boost::program_options::value <std::vector <std::string>>(&mParsedArgs),
            "argument for app execution")
        ("input,i", boost::program_options::value <std::string>(),
            "input file path")
        ("output,o", boost::program_options::value <std::string>(),
            "output file path");
//        ("test-save", boost::program_options::bool_switch(),
//            "save test after execution");
}

void CAppExecutor::run()
{
    emit started();
    boost::program_options::variables_map varMap;
    if(!readOptions(mArgs, varMap))
    {
        emit finished(1);
        return;
    }

    QString codePath = QString::fromStdString(varMap["src"].as<std::string>());
    codePath = NFileSystem::get_full_path(codePath);
    ProgLanguage::EType language = parseProgLanguage(varMap);

    QStringList flags;
    for(const std::string& str : mParsedFlags)
    {
        flags << QString::fromStdString("-" + str);
    }
    QStringList args;
    for(const std::string& str : mParsedArgs)
    {
       args << QString::fromStdString(str);
    }

    mForcedCompilation = varMap["force"].as<bool>();
    if(varMap.count("input"))
    {
       mInputFilePath = NFileSystem::get_full_path(
                QString::fromStdString(varMap["input"].as<std::string>())).toStdString();
    }
    if(varMap.count("output"))
    {
       mOutputFilePath = NFileSystem::get_full_path(
                QString::fromStdString(varMap["output"].as<std::string>())).toStdString();
    }

    compileCode(codePath, flags, args, language);
}

void CAppExecutor::appendData(const QString& str)
{
    qDebug () << "CAppExecutor::appendData() " << str;
    mProcess->write(str.toLocal8Bit());
    mProcess->write("\n");
}

void CAppExecutor::setArgs(const QStringList &args)
{
    mArgs = args;
}

void CAppExecutor::terminate()
{
    assert(mProcess != nullptr);

    qDebug () << "CAppExecutor::terminate()";
    disconnect(mErrorConnection);
    mProcess->terminate();
}

void CAppExecutor::compileCode(const QString& codePath,
                               const QStringList& flags,
                               const QStringList& args,
                               ProgLanguage::EType lang)
{
    CCompiler* compiler = new CCompiler({CCompiler::SCompilerTask(codePath, flags, lang, mForcedCompilation)});
    connect(compiler, &CCompiler::log, [this](const QString& msg){
        emit log(msg);
    });
    connect(compiler, &CCompiler::error, [this](const QString& msg){
        emit error(msg);
    });
    connect(compiler, &CCompiler::finished, [this, compiler, args](int code){
        if(code == 0)
        {
            QString appPath = compiler->getResult().front();
            runApp(appPath, args);
        }
        else
        {
            emit finished(1);
        }
        compiler->deleteLater();
    });
    compiler->run();
}

void CAppExecutor::runApp(const QString& appPath, const QStringList& args)
{
    if(!mOutputFilePath.empty())
    {
        mOutputFile.open(mOutputFilePath);
        if(!mOutputFile.is_open())
        {
            emit error(" [ Error ] failed to open " + QString::fromStdString(mOutputFilePath)
                      + "for writing\n");
            emit finished(1);
            return;
        }
    }
    QString inputData;
    if(!mInputFilePath.empty())
    {
       std::ifstream file(mInputFilePath);
       if(!file.is_open())
       {
          emit error(" [ Error ] failed to open " + QString::fromStdString(mInputFilePath)
                    + "for reading\n");
          emit finished(1);
          return;
       }
       std::string buffer;
       while(std::getline(file, buffer))
       {
          inputData += QString::fromStdString(buffer) + "\n";
       }
    }

    mProcess = new QProcess();
    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
       QString msg = mProcess->readAllStandardOutput();
       qDebug () << "mProcess log " << msg;
       if(mOutputFilePath.empty())
       {
           emit log(msg);
       }
       else
       {
           mOutputFile << msg.toStdString();
       }
    });
    connect(mProcess, &QProcess::readyReadStandardError, [this](){
       emit error(mProcess->readAllStandardError());
    });
    mProcess->setWorkingDirectory(mWorkingDirectory);

    connect(mProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
            [this](int code)
    {
       emit finished(code);
       mProcess->deleteLater();
       mProcess = nullptr;
    });
    //mErrorConnection = connect(mProcess, &QProcess::errorOccurred, [this](QProcess::ProcessError err){
    mErrorConnection = connect(mProcess, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
                               [this](QProcess::ProcessError err){
        emit error(processErrorToStr(err) + "\n");
        emit finished(1);
        mProcess->deleteLater();
        mProcess = nullptr;
    });

    mProcess->start(appPath, args, QProcess::Unbuffered | QProcess::ReadWrite);
    mProcess->waitForStarted();
    if(!inputData.isEmpty())
    {
        appendData(inputData);
        mProcess->closeWriteChannel();
    }
}

ProgLanguage::EType CAppExecutor::parseProgLanguage(
        boost::program_options::variables_map& varMap)
{
    if(varMap.count("lang"))
    {
        return getProgLanguageType(varMap["lang"].as<std::string>());
    }
    else
    {
        QString extension = NFileSystem::get_file_extension(
                    QString::fromStdString(varMap["src"].as<std::string>()));
        return getProgLanguageType(extension.toStdString());
    }
}


} // namespace NCommand
