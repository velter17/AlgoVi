/**
 * Project   AlgoVi
 *
 * @file     CCompiler.cpp
 * @author   Dmytro Sadovyi
 * @date     02.05.2017
 */

#include <QDebug>

#include "../CCompiler.hpp"
#include "framework/commands/private/CCompilerExecutor.hpp"

namespace NCommand
{

CCompiler::CCompiler(const std::vector<SCompilerTask>& tasks)
    : mTasks(tasks)
{
    qDebug () << "CCompiler::CCompiler, tasks: ";
    for(int i = 0; i < mTasks.size(); ++i)
    {
        qDebug () << mTasks[i].mCodePath << " " << mTasks[i].mFlags;
    }
}

CCompiler::~CCompiler()
{
   qDebug () << "~CCompiler()";
}

void CCompiler::run()
{
    emit started();
    compile(0);
}

std::vector<QString> CCompiler::getResult()
{
    return mResult;
}

void CCompiler::compile(int index)
{
    if(mTasks.size() <= index)
    {
        emit finished(0);
        return;
    }
    qDebug () << "CCompiler::compile " << index << " " << mTasks[index].mCodePath;
    CCompilerExecutor* executor = new CCompilerExecutor;
    connect(executor, &CCompilerExecutor::log, [this](const QString& msg){
        emit log(msg);
    });
    connect(executor, &CCompilerExecutor::error, [this](const QString& msg){
        emit error(msg);
    });
    connect(executor, &CCompilerExecutor::finished, [this, index, executor](const QString& appPath){
        executor->deleteLater();
        if(appPath.isEmpty())
        {
            emit finished(1);
        }
        else
        {
            mResult.push_back(appPath);
            compile(index+1);
        }
    });
    executor->compile(mTasks[index].mCodePath, mTasks[index].mFlags, mTasks[index].mLanguage, mTasks[index].mForced);
}

} // namespace NCommand
