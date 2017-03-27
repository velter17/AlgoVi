/**
 * Project   AlgoVi
 *
 * @file     CTerminalCommandBase.hpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#pragma once

#include <boost/program_options.hpp>

#include "framework/commands/ITerminalCommand.hpp"

namespace NCommand
{

class CTerminalCommandBase : public ITerminalCommand
{
public: // methods
    CTerminalCommandBase(QStringList args = QStringList());

    void setWorkingDirectory(const QString& dir) override;
    void setWorkingTime(uint32_t time) override;

    QStringList getOptionsList();

protected: // methods
    QString getManualMessage() override;

protected: // methods
    QString getOptionsHelp();
    bool readOptions(const QStringList& args,
                     boost::program_options::variables_map& varMap);

protected: // fields
    QString mWorkingDirectory;
    boost::program_options::options_description mOptions;
    boost::program_options::positional_options_description mOptionsPositional;
    QStringList mArgs;
    uint32_t mWorkingTime;
};

} // namespace NCommand

