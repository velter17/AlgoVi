/**
 * Project   AlgoVi
 *
 * @file     CAlgoViSystem.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <QString>

#include "framework/commands/ITerminalSignals.hpp"

namespace NAlgoVi
{

class CAlgoViSystem : public NCommand::ITerminalSignals
{
   Q_OBJECT
public: // methods
    CAlgoViSystem();

public slots:
    void executeCommand(const QString& command);

signals:
    void log(const QString& str);
    void error(const QString& str);
    void logHtml(const QString& str);
    void finishedCommand();
};

} // namespace NAlgoVi

