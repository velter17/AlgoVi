/**
 * Project   AlgoVi
 *
 * @file     CTerminalCommandBase.cpp
 * @author   Dmytro Sadovyi
 * @date     28.03.2017
 */

#include <QDebug>

#include "framework/commands/CTerminalCommandBase.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

const uint32_t sDefaultWorkingTime = 60 * 60 * 100; // minute

CTerminalCommandBase::CTerminalCommandBase()
    : mWorkingTime(sDefaultWorkingTime)
    , mWorkingDirectory(NFileSystem::get_current_dir())
{
    mOptions.add_options()
        ("help,h", "show help message")
          ("manual,?", "detailed help message (manual)");
}

CTerminalCommandBase::~CTerminalCommandBase()
{

}

void CTerminalCommandBase::appendData(const QString& str)
{
   // for commands, which have not interactions
   qDebug () << "Warning: unexpected appendData function calling";
}

void CTerminalCommandBase::terminate()
{
   // for commands, which have not been terminated
   qDebug () << "Warning: unexpected terminate function calling";
}

void CTerminalCommandBase::setWorkingDirectory(const QString& dir)
{
    mWorkingDirectory = dir;
}

void CTerminalCommandBase::setWorkingTime(uint32_t time)
{
   mWorkingTime = time;
}

void CTerminalCommandBase::setArgs(const QStringList& args)
{
   // for commands, which have not arguments
   qDebug () << "Warning: unexpected setArgs function calling";
}

QStringList CTerminalCommandBase::getOptionsList()
{
    QStringList ret;
    for(const auto itr : mOptions.options())
    {
        ret.append("--" + QString::fromStdString(itr->canonical_display_name()));
    }
    return ret;
}

QStringList CTerminalCommandBase::getOptionValues(const QString &option)
{
   if(mOptionValues.find(option) == mOptionValues.end())
   {
      return QStringList();
   }
   return mOptionValues[option];
}

QString CTerminalCommandBase::getManualMessage()
{
    return "Sorry, no manual message :(";
}

QString CTerminalCommandBase::getOptionsHelp()
{
    std::stringstream sstream;
    sstream << mOptions;
    return "Allowed options:\n" + QString::fromStdString(sstream.str());
}

bool CTerminalCommandBase::readOptions(const QStringList& args, boost::program_options::variables_map& varMap)
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
        return false;
    }
    return true;
}

} // namespace NCommand
