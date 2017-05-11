/**
 * Project   AlgoVi
 *
 * @file     CParserCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     11.05.2017
 */

#pragma once

#include "algovi/system/jobs/IJob.hpp"

namespace NController
{
class CController;
}

namespace NCommand
{
class CParserCommand;
}

namespace NAlgoVi
{

class CParserCommand : public IJob
{
public: // methods
    CParserCommand(NController::CController* controller);
    void run(const QStringList& args) override;
    void appendData(const QString& data) override;
    void terminate() override;
    QStringList getArguments() override;

private: // fields
    NCommand::CParserCommand* mParser;
    NController::CController* mControllerPtr;
};

} // namespace NAlgoVi

