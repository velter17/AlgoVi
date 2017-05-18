/**
 * Project   AlgoVi
 *
 * @file     ITerminal.hpp
 * @author   Dmytro Sadovyi
 * @date     27.03.2017
 */

#pragma once

#include <QObject>
#include <QString>
#include <memory>

#include "framework/complation/CComplationProvider.hpp"
#include "framework/common/ClassGenerators.hpp"

namespace NController
{

class CommandSettings
{
ADD_ATTRIBUTE(CommandSettings, CtrlLogic, bool)
};

class ITerminal : public QObject
{
   Q_OBJECT
public slots:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void setQuestionMode() = 0;
    virtual void setInsideProcessMode() = 0;
    virtual void setCommandSettings(const CommandSettings& settings) = 0;

    virtual void appendSimpleText(const QString& text) = 0;
    virtual void appendHtmlText(const QString& text) = 0;
    virtual void appendErrorText(const QString& text) = 0;

    virtual void setComplation(std::shared_ptr<NCommand::CComplationProvider> provider) = 0;
signals:
    virtual void command(const QString& cmd);
    virtual void newData(const QString& cmd);
    virtual void termination();
};

} // namespace NController
