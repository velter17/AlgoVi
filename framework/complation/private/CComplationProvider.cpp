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

QStringList extractArgumentsList(const tArgumentsContainer& container)
{
   return container.keys();
}

CComplationProvider::CComplationProvider()
{

}

void CComplationProvider::addCommand(const QString& name)
{
   mCommandList.append(name);
}

void CComplationProvider::addCommand(const QString& name, const tArgumentsContainer& args)
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
          int idx = NCommandParser::isArgumentValue(last);
          if(idx == -1 || idx < 2 ||
                mArgumentsList[commandName].find(last.mid(0, idx)) == mArgumentsList[commandName].end())
          {
             lastWordLen = last.length();
             return NCommandParser::processHint(extractArgumentsList(mArgumentsList[commandName]), last);
          }
          else
          {
             lastWordLen = last.length() - idx - 1;
             QStringList ret = NCommandParser::processHint(mArgumentsList[commandName][last.mid(0, idx)], last.mid(idx+1));
             if(ret.isEmpty())
             {
                return NCommandParser::filesystemComplation(last.mid(idx+1), lastWordLen);
             }
             else
             {
                return ret;
             }
          }
      }
      else
      {
          return NCommandParser::filesystemComplation(last, lastWordLen);
      }
   }
}


} // namespace NCommand
