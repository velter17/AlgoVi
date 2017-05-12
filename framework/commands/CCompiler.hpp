/**
 * Project   AlgoVi
 *
 * @file     CCompiler.hpp
 * @author   Dmytro Sadovyi
 * @date     02.05.2017
 */

#pragma once

#include "framework/commands/CTerminalCommandBase.hpp"
#include "framework/commands/TProgLanguages.hpp"

namespace NCommand
{

class CCompiler : public CTerminalCommandBase
{
public: // types
    struct SCompilerTask
    {
        QString mCodePath;
        QStringList mFlags;
        ProgLanguage::EType mLanguage;
        bool mForced;
        SCompilerTask(const QString& codePath,
                      const QStringList& flags,
                      ProgLanguage::EType lang,
                      bool forced = false)
            : mCodePath(codePath)
            , mFlags(flags)
            , mLanguage(lang)
            , mForced(forced)
        {}
    };
public: // methods
    CCompiler(const std::vector <SCompilerTask>& tasks);
    ~CCompiler();
    void run() override;
    std::vector <QString> getResult();

private: // methods
    void compile(int index);

private: // fields
    std::vector <QString> mResult;
    std::vector <SCompilerTask> mTasks;
};

} // namespace NCommand
