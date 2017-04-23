/**
 * Project   AlgoVi
 *
 * @file     CAppExecutor.cpp
 * @author   Dmytro Sadovyi
 * @date     12.04.2017
 */

#include <QDebug>

#include "../CAppExecutor.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

ProgLanguage::EType getProgLanguageType(const std::string& type)
{
    if(type == "cpp")
    {
        return ProgLanguage::CPP;
    }
    else if(type == "java")
    {
        return ProgLanguage::JAVA;
    }
    else if(type == "py")
    {
        return ProgLanguage::PYTHON;
    }
    else
    {
        return ProgLanguage::BINARY;
    }
}

QString processErrorToStr(QProcess::ProcessError err)
{
    if(err == QProcess::ProcessError::FailedToStart)
    {
        return "Failed to start";
    }
    else if(err == QProcess::ProcessError::Crashed)
    {
        return "Crashed";
    }
    else if(err == QProcess::ProcessError::Timedout)
    {
        return "Timedout";
    }
    else if(err == QProcess::ProcessError::ReadError)
    {
        return "Read error";
    }
    else if(err == QProcess::ProcessError::WriteError)
    {
        return "Write error";
    }
    else
    {
        return "Unknown error";
    }
}

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
            "output file path")
        ("test-save", boost::program_options::bool_switch(),
            "save test after execution");
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

    mProcess = new QProcess();
    connect(mProcess, &QProcess::readyReadStandardOutput, [this](){
       emit log(mProcess->readAllStandardOutput());
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
//    connect(mProcess, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
//            [this](int code, QProcess::ExitStatus status)
//    {
//        emit finished(code);
//        mProcess->deleteLater();
//        mProcess = nullptr;
//    });
    mErrorConnection = connect(mProcess, &QProcess::errorOccurred, [this](QProcess::ProcessError err){
        emit error(processErrorToStr(err) + "\n");
        emit finished(-1);
        mProcess->deleteLater();
        mProcess = nullptr;
    });


    QString appPath = QString::fromStdString(varMap["src"].as<std::string>());
    appPath = NFileSystem::get_full_path(appPath);
    mProcess->start(appPath, QProcess::Unbuffered | QProcess::ReadWrite);
}

void CAppExecutor::appendData(const QString &str)
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
