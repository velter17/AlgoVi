/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.cpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#include <QDebug>
#include <QFile>
#include <iostream>

#include "framework/commands/executor/CAppExecutor.hpp"
#include "framework/commands/CCompiler.hpp"
#include "framework/filesystem/filesystem.hpp"
#include "framework/commands/ProcessHelper.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/commands/executor/DebugHelper.hpp"


namespace NCommand
{

CAppExecutor::CAppExecutor()
   : mOutputType(OutputType::NoSpecified)
   , mTimeLimitExceeded(false)
   , mCrashed(false)
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
            "output file path")
        ("test,t", boost::program_options::value <int>(), "run on test")
        ("time-limit", boost::program_options::value<int>(), "execute no more then [value in ms]")
        ("debug", boost::program_options::bool_switch()->default_value(false),
            "apply colors")
        ("remove-debug", boost::program_options::bool_switch()->default_value(false),
            "execute without debug instructions");
//        ("test-save", boost::program_options::bool_switch(),
    //            "save test after execution");

    mOptionValues["--lang"].append(getProgLanguageList());
}

CAppExecutor::~CAppExecutor()
{
    qDebug () << "~CAppExecutor " << mArgs;
}

void CAppExecutor::run()
{
    emit started();
    boost::program_options::variables_map varMap;
    if(!readOptions(mArgs, varMap))
    {
        emit finished(static_cast<int>(ReturnCodes::CompilationError));
        return;
    }

    QString codePath = QString::fromStdString(varMap["src"].as<std::string>());
    codePath = NFileSystem::get_full_path(codePath);
    if(!NFileSystem::exists(codePath))
    {
       emit error(" [ Error ] Wrong source code - no such file\n");
       emit finished(static_cast<int>(ReturnCodes::CompilationError));
       return;
    }

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
       if(!NFileSystem::exists(QString::fromStdString(mInputFilePath)))
       {
          emit error(" [ Error ] Wrong input file - no such file\n");
          emit finished(static_cast<int>(ReturnCodes::CompilationError));
          return;
       }
    }
    if(varMap.count("output"))
    {
       mOutputFilePath = NFileSystem::get_full_path(
                QString::fromStdString(varMap["output"].as<std::string>())).toStdString();
    }

    mTestToRun = varMap.count("test") ? varMap["test"].as<int>() : -1;
    mDebugMode = varMap["debug"].as<bool>();
    mRemoveDebug = varMap["remove-debug"].as<bool>();

    mTimeLimit = varMap.count("time-limit")
             ? varMap["time-limit"].as<int>()
             : 0;
    mTimeLimit = mTimeLimit <= 0 ? 1000 * 60 * 60 : mTimeLimit;

    compileCode(codePath, flags, args, language);
}

void CAppExecutor::appendData(const QString& str)
{
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

void CAppExecutor::setOutputType(OutputType::EType type)
{
   mOutputType = type;
}

QString CAppExecutor::getOutput()
{
   return mOutputStorage;
}

uint32_t CAppExecutor::getExecutionTime()
{
   return mExecutionTime;
}

void CAppExecutor::compileCode(const QString& codePath,
                               const QStringList& flags,
                               const QStringList& args,
                               ProgLanguage::EType lang)
{
    QString codeToCompile;
    if(mRemoveDebug)
    {
       QString msg = clearCodePath(codePath, codeToCompile);
       if(!msg.isEmpty())
       {
          emit error(" [ Error ] " + msg + "\n");
          emit finished(static_cast<int>(ReturnCodes::CompilationError));
          return;
       }
    }
    else
    {
       codeToCompile = codePath;
    }
    CCompiler* compiler = new CCompiler({CCompiler::SCompilerTask(codeToCompile, flags, lang, mForcedCompilation)});
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
            emit finished(static_cast<int>(ReturnCodes::CompilationError));
        }
        compiler->deleteLater();
    });
    compiler->run();
}

void CAppExecutor::runApp(const QString& appPath, const QStringList& args)
{
    qDebug () << "CAppExecutor::runApp " << appPath << " " << args;
    if(!mOutputFilePath.empty())
    {
        mOutputFile.open(mOutputFilePath);
        if(!mOutputFile.is_open())
        {
            emit error(" [ Error ] failed to open " + QString::fromStdString(mOutputFilePath)
                      + "for writing\n");
            emit finished(static_cast<int>(ReturnCodes::CompilationError));
            return;
        }
        mOutputType = OutputType::ToFile;
        qDebug () << "CAppExecutor output to file " << QString::fromStdString(mOutputFilePath);
    }
    else if(mOutputType == OutputType::NoSpecified)
    {
        mOutputType = OutputType::JustEmit;
    }
    QString inputData;
    if(mTestToRun != -1)
    {
       if(CTestProvider::getInstance().size() < mTestToRun)
       {
          emit error(" [ Error ] wrong test to run\n");
          emit finished(static_cast<int>(ReturnCodes::CompilationError));
          return;
       }
       inputData = CTestProvider::getInstance().getTest(mTestToRun-1).first + "\n";
    }
    else if(!mInputFilePath.empty())
    {
       std::ifstream file(mInputFilePath);
       if(!file.is_open())
       {
          emit error(" [ Error ] failed to open " + QString::fromStdString(mInputFilePath)
                    + "for reading\n");
          emit finished(static_cast<int>(ReturnCodes::CompilationError));
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
       if(mOutputType == OutputType::JustEmit)
       {
          if(mDebugMode)
          {
             emit logHtml(processDebugStr(msg));
          }
          else
          {
             emit log(msg);
          }
       }
       else if(mOutputType == OutputType::ToFile)
       {
          mOutputFile << msg.toStdString();
       }
       else if(mOutputType == OutputType::ToStorage)
       {
          mOutputStorage += msg;
       }
    });
    connect(mProcess, &QProcess::readyReadStandardError, [this](){
        emit error(mProcess->readAllStandardError());
    });
    mProcess->setWorkingDirectory(mWorkingDirectory);
    connect(mProcess, static_cast<void(QProcess::*)(int)>(&QProcess::finished),
            [this](int code)
    {
        qDebug () << "CAppExecutor: mProcess finished with code " << code;
        qDebug () << "remaining time: " << mTimer.remainingTime() << ", timeLimit = " << mTimeLimit;
        mExecutionTime = mTimeLimit - mTimer.remainingTime();
        mTimer.stop();
        if(mOutputType == OutputType::ToFile)
        {
            mOutputFile.close();
        }
        mProcess->deleteLater();
        mProcess = nullptr;
        int codeToFinish = mTimeLimitExceeded
              ? static_cast<int>(ReturnCodes::TimeLimitExceeded)
              : static_cast<int>(code == 0 && !mCrashed ? ReturnCodes::Success : ReturnCodes::RuntimeError);
        emit finished(codeToFinish);
    });
    mErrorConnection = connect(mProcess, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
                               [this](QProcess::ProcessError err){
        qDebug () << "CAppExecutor: mProcess emitted error" << processErrorToStr(err);
        if(err == QProcess::FailedToStart)
        {
            mExecutionTime = mTimeLimit - mTimer.remainingTime();
            mTimer.stop();
            emit error(processErrorToStr(err) + "\n");
            mProcess->deleteLater();
            mProcess = nullptr;
            emit finished(static_cast<int>(ReturnCodes::RuntimeError));
         }
         else
         {
            disconnect(mErrorConnection);
            mCrashed = true;
         }
    });

    mTimer.setInterval(mTimeLimit);
    mTimer.setSingleShot(true);
    mTimer.setTimerType(Qt::PreciseTimer);
    connect(&mTimer, &QTimer::timeout, [this](){
       terminate();
       mTimeLimitExceeded = true;
    });
    connect(mProcess, SIGNAL(started()), &mTimer, SLOT(start()));

    qDebug () << "start " << appPath << ", args: " << args;
    if(mDebugMode)
    {
       mProcess->setProcessChannelMode(QProcess::MergedChannels);
    }
    if(!inputData.isEmpty())
    {
       connect(mProcess, &QProcess::started, [this, inputData](){
          appendData(inputData);
          mProcess->closeWriteChannel();
       });
    }
    mProcess->start(appPath, args, QProcess::Unbuffered | QProcess::ReadWrite);
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
