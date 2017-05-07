/**
 * Project   AlgoVi
 *
 * @file     CComplationProvider.cpp
 * @author   Dmytro Sadovyi
 * @date     05.05.2017
 */

#include <QDebug>

#include "framework/complation/CComplationProvider.hpp"
#include "framework/complation/TCommandParserHelper.hpp"

namespace NCommand
{

CComplationProvider::CComplationProvider()
{

}

void CComplationProvider::addCommand(const QString& name)
{
   mCommandList.append(name);
}

void CComplationProvider::addCommand(const QString& name, const QStringList& args)
{
   mCommandList.append(name);
   mArgumentsList.insert(name, args);
}

QStringList CComplationProvider::complation(const QString& cmd, int& lastWordLen)
{
   qDebug () << "CComplationProvider::completion " << cmd;
   QStringList cmdList = NCommandParser::split(cmd);
   if(cmdList.isEmpty()) // just empty command string
   {
      lastWordLen = 0;
      return QStringList();
   }
   if(cmdList.size() == 1) // command name complation
   {
      const QString& str = cmdList.first();
      lastWordLen = str.length();
      return NCommandParser::processHint(mCommandList, str);
   }
   else // command arguments complation [ or filesystem path ]
   {
      const QString& commandName = cmdList.first();
      const QString& last = cmdList.last();
      if(mArgumentsList.find(commandName) != mArgumentsList.end() && NCommandParser::isArgument(last))
      {
          lastWordLen = last.length();
          return NCommandParser::processHint(mArgumentsList[commandName], last);
      }
      else
      {
          return NCommandParser::filesystemComplation(last, lastWordLen);
      }
   }
}


} // namespace NCommand
