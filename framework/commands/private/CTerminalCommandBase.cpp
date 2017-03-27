/**
 * Project   AlgoVi
 *
 * @file     CTerminalCommandBase.cpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#include "framework/commands/CTerminalCommandBase.hpp"

namespace NCommand
{

const uint32_t sDefaultWorkingTime = 60 * 60 * 100; // minute

CTerminalCommandBase::CTerminalCommandBase(QStringList args)
    : mArgs(args)
    , mWorkingTime(sDefaultWorkingTime)
{
    mOptions.add_options()
        ("help,h", "show help message")
        ("manual,?", "detailed help message (manual)");
}

void CTerminalCommandBase::setWorkingDirectory(const QString &dir)
{
    mWorkingDirectory = dir;
}

void CTerminalCommandBase::setWorkingTime(uint32_t time)
{
    mWorkingTime = time;
}

QStringList CTerminalCommandBase::getOptionsList()
{
    QStringList ret;
    for(const auto itr : mOptions.options())
    {
        ret.append(QString::fromStdString(itr->canonical_display_name()));
    }
    return ret;
}

QString CTerminalCommandBase::getManualMessage()
{
    return "Sorry, no help message :(";
}

QString CTerminalCommandBase::getOptionsHelp()
{
    std::stringstream sstream;
    sstream << mOptions;
    return "Allowed options:\n" + QString::fromStdString(sstream.str());
}

bool CTerminalCommandBase::readOptions(const QStringList &args, boost::program_options::variables_map &varMap)
{
    try
    {
        std::vector <std::string> vargs;
        for(const QString& str : args)
        {
            vargs.push_back(str.toStdString());
        }
        boost::program_options::parsed_options parser =
                boost::program_options::command_line_parser(vargs)
                .options(mOptions)/*.positional(mOptionsPositional)*/.run();
        boost::program_options::store(parser, varMap);
    }
    catch(const boost::program_options::error& e)
    {
        emit error(QString::fromLocal8Bit(e.what()));
        return false;
    }

    if(varMap.count("help"))
    {
        emit log(getOptionsHelp());
        return false;
    }
    if(varMap.count("manual"))
    {
        emit logHtml(getManualMessage());
        return false;
    }

    try
    {
        boost::program_options::notify(varMap);
    }
    catch(const boost::program_options::error& e)
    {
        emit error(QString::fromLocal8Bit(e.what()));
        emit log(getManualMessage());
        return false;
    }
    return true;
}

} // namespace NCommand
