/**
 * Project   AlgoVi
 *
 * @file     CAlgoViSystem.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <QString>
#include <memory>

#include "algovi/system/CCommandHandler.hpp"
#include "framework/commands/ITerminalSignals.hpp"

namespace NAlgoVi
{

class CAlgoViSystem : public NCommand::ITerminalSignals
{
   Q_OBJECT
public: // methods
    CAlgoViSystem(NController::CController* controller);

public slots:
    void executeCommand(const QString& command);
    void appendData(const QString& data);
    void terminateJob();

signals:
    void log(const QString& str);
    void error(const QString& str);
    void logHtml(const QString& str);
    void finishedCommand();
private: // fields
    NController::CController* mControllerPtr;
    CCommandHandler mCommandHandler;
    std::shared_ptr<IJob> mJobPtr;
};

} // namespace NAlgoVi

