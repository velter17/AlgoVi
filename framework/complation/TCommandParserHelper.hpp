/**
 * Project   AlgoVi
 *
 * @file     TCommandParserHelper.hpp
 * @author   Dmytro Sadovyi
 * @date     05.05.2017
 */

#pragma once

#include <QStringList>

#include "framework/filesystem/filesystem.hpp"

namespace NCommandParser
{

inline QStringList split(const QString& cmd)
{
   return cmd.split(QRegExp("\\s+"), QString::SkipEmptyParts);
}

inline int isArgument(const QString& cmd)
{
   return cmd.length() > 1 && cmd.mid(0, 2) == "--";
}

inline QStringList processHint(const QStringList& list, const QString& arg)
{
   QStringList ret;
   for(const QString& s : list)
   {
      if(s.length() >= arg.length() && s.mid(0, arg.length()) == arg)
      {
         ret.append(s);
      }
   }
   if(ret.size() == 1)
   {
      QString s = *ret.begin();
      ret.clear();
      ret.push_back(s.mid(arg.length()));
   }
   return ret;
}

inline QStringList filesystemComplation(const QString& arg, int& lastWordLen)
{
   QString path = NFileSystem::get_full_path(arg);
   qDebug () << "filesystemCompletion: " << arg << " -> " << path;
   if(arg.endsWith('/') && NFileSystem::is_directory(arg))
   {
       lastWordLen = 0;
       QStringList ret = NFileSystem::content_list(arg);
       if(ret.size() == 1 && NFileSystem::is_directory(path + "/" + ret.first()))
       {
          ret.first().append('/');
       }
       return ret;
   }
   else
   {
      QString filePath = NFileSystem::get_file_path(arg);
      QString fileName = NFileSystem::get_file_name(arg);
      QStringList ret = processHint(NFileSystem::content_list(filePath), fileName);
      if(ret.size() == 1 && NFileSystem::is_directory(arg + ret.first()))
      {
          ret.first().append('/');
      }
      lastWordLen = fileName.length();
      return ret;
   }
}

} // namespace NCommandParser

