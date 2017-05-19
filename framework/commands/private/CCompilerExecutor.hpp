/**
 * Project   AlgoVi
 *
 * @file     CCompilerExecutor.hpp
 * @author   Dmytro Sadovyi
 * @date     02.05.2017
 */

#pragma once

#include "framework/commands/ITerminalSignals.hpp"

namespace NCommand
{

class CCompilerExecutor : public ITerminalSignals
{
    Q_OBJECT
public: // methods
    CCompilerExecutor();
    void compile(const QString& codePath,
                 const QStringList& flags,
                 const QString& lang,
                 bool forced);

signals:
    void log(const QString& str);
    void logHtml(const QString& str);
    void error(const QString& str);
    void finished(const QString& appPath);
    void started();
};

} // namespace NCommand

