/**
 * Project   AlgoVi
 *
 * @file     CCommandHandler.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <QMap>
#include <memory>

#include "algovi/system/jobs/IJob.hpp"

namespace NController
{
   class CController;
}

namespace NAlgoVi
{

struct CommandType
{
   enum EType
   {
      Wrong,
      ForTest,
      Empty,
      Exit,

      Total,
   };
};

class CCommandHandler
{
public: // methods
    CCommandHandler(NController::CController* controller);
    std::shared_ptr <IJob> getJob(const QString& cmd);

private: // types
    typedef std::shared_ptr<IJob> (CCommandHandler::*TCreator)();

private: // methods
    template <CommandType::EType command>
    std::shared_ptr<IJob> jobCreator();

    template <CommandType::EType command>
    void jobCreatorRegistrator();


private: // fields
    NController::CController* mControllerPtr;
    TCreator mCreatorFunctions[(std::size_t)CommandType::Total];
    QMap<QString, CommandType::EType> mCommandMap;
};

} // namespace NAlgoVi

