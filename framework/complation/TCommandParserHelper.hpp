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

inline QStringList filesystemComplation(const QString& arg)
{
   QString path = NFileSystem::get_full_path(arg);
   qDebug () << "filesystemCompletion: " << arg << " -> " << path;
   if(NFileSystem::is_directory(arg))
   {
      return NFileSystem::content_list(arg);
   }
   else
   {
      QString filePath = NFileSystem::get_file_path(arg);
      return processHint(NFileSystem::content_list(filePath), NFileSystem::get_file_name(arg));
   }
}

} // namespace NCommandParser

