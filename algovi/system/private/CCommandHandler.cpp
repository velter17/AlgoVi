/**
 * Project   AlgoVi
 *
 * @file     CCommandHandler.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include <QStringList>

#include "algovi/system/jobs/CAppExecutor.hpp"
#include "algovi/system/jobs/CEmptyJob.hpp"
#include "algovi/system/jobs/CJobForTest.hpp"
#include "algovi/system/jobs/CExitCommandJob.hpp"
#include "algovi/system/jobs/CWrongCommandJob.hpp"
#include "algovi/system/jobs/CPythonInterpreter.hpp"
#include "algovi/system/jobs/CInternalSystemCommand.hpp"
#include "algovi/system/jobs/CChangeDirCommand.hpp"
#include "controller/CController.hpp"
#include "../CCommandHandler.hpp"

namespace NAlgoVi
{

CCommandHandler::CCommandHandler(NController::CController *controller)
   : mControllerPtr(controller)
{
   jobCreatorRegistrator<(CommandType::EType)0>();

   mCommandMap.insert("test_command",        CommandType::ForTest);
   mCommandMap.insert("",                    CommandType::Empty);
   mCommandMap.insert("exit",                CommandType::Exit);
   mCommandMap.insert("python",              CommandType::Python);
   mCommandMap.insert("cd",                  CommandType::ChangeDir);
   mCommandMap.insert("run",                 CommandType::ExecuteApp);

   for(const QString& cmd : CInternalSystemCommand::getCommandList())
   {
       mCommandMap.insert(cmd, CommandType::InternalSystem);
   }
}

template <CommandType::EType command>
void CCommandHandler::jobCreatorRegistrator()
{
   mCreatorFunctions[(std::size_t)command] = &CCommandHandler::jobCreator<command>;
   jobCreatorRegistrator<(CommandType::EType)((std::size_t)command + 1)>();
}

template <>
void CCommandHandler::jobCreatorRegistrator<CommandType::Total>()
{
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::Empty>()
{
   return std::make_shared<CEmptyJob>();
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::Exit>()
{
   return std::make_shared<CExitCommandJob>(mControllerPtr);
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::ForTest>()
{
   return std::make_shared<CJobForTest>(mControllerPtr);
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::Wrong>()
{
    return std::make_shared<CWrongCommandJob>(mControllerPtr);
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::Python>()
{
   return std::make_shared<CPythonInterpreter>(mControllerPtr);
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::InternalSystem>()
{
   return std::make_shared<CInternalSystemCommand>(mControllerPtr);
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::ChangeDir>()
{
   return std::make_shared<CChangeDirCommand>(mControllerPtr);
}

template <>
std::shared_ptr<IJob> CCommandHandler::jobCreator<CommandType::ExecuteApp>()
{
   return std::make_shared<CAppExecutor>(mControllerPtr);
}

std::shared_ptr<IJob> CCommandHandler::getJob(const QString& cmd)
{
   CommandType::EType type = CommandType::Wrong;
   if(mCommandMap.find(cmd) != mCommandMap.end())
   {
      type = mCommandMap[cmd];
   }
   return (this->*mCreatorFunctions[(std::size_t)type])();
}


} // namespace NAlgoVi
