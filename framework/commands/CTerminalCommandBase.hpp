/**
 * Project   AlgoVi
 *
 * @file     CTerminalCommandBase.hpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#pragma once

#include <QMap>
#include <boost/program_options.hpp>

#include "framework/commands/ITerminalCommand.hpp"

namespace NCommand
{

class CTerminalCommandBase : public ITerminalCommand
{
public: // methods
    CTerminalCommandBase();
    ~CTerminalCommandBase();

    void appendData(const QString& str) override;
    void terminate() override;
    void setWorkingDirectory(const QString& dir) override;
    void setWorkingTime(uint32_t time) override;
    void setArgs(const QStringList& args);

    CSettings getSettings() override;

    QStringList getOptionsList();
    QStringList getOptionValues(const QString& option);

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
    QMap<QString, QStringList> mOptionValues;
    uint32_t mWorkingTime;
    CSettings mSettings;
};

} // namespace NCommand

