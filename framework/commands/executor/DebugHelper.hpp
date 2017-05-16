/**
 * Project   AlgoVi
 *
 * @file     DebugHelper.hpp
 * @author   Dmytro Sadovyi
 * @date     16.05.2017
 */

#pragma once

#include <QFile>
#include <QString>

#include "framework/common/HtmlConverter.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

inline QString preprocessCode(const QString& codePath, const QString& destPath)
{
   QString codeFolder = NFileSystem::get_file_path(codePath);
   QString debugFile = codeFolder + "/algovi_debug.h";
   QFile file(debugFile);
   if(!file.open(QFile::ReadOnly))
   {
      return "can't open algovi_debug.h";
   }

   QString keyWord = file.readLine();
   if(!keyWord.isEmpty())
   {
      keyWord.chop(1);
   }
   QStringList list = keyWord.split(' ', QString::SkipEmptyParts);
   if(list.size() < 2 || (list.first() != "#define"))
   {
      return "wrong keyWord line in algovi_debug.h, line = " + keyWord;
   }
   else
   {
      keyWord = list.at(1);
      file.close();
   }

   file.setFileName(codePath);
   if(!file.open(QFile::ReadOnly))
   {
      return "can't open " + codePath;
   }
   QFile destFile(destPath);
   if(!destFile.open(QFile::WriteOnly))
   {
      return "can't create " + destPath;
   }

   bool debugOutput = false;
   while(true)
   {
      QString str = file.readLine();
      if(str.isEmpty())
      {
         break;
      }
      int idx = 0;
      while(idx < str.length() && str[idx] == QChar::Tabulation || str[idx] == QChar::Space)
      {
         ++idx;
      }
      if(str.mid(idx).startsWith(keyWord + "_start"))
      {
         debugOutput = true;
      }
      else if(str.mid(idx).startsWith(keyWord + "_end"))
      {
         debugOutput = false;
      }
      if(debugOutput || str.mid(idx).startsWith(keyWord) || str.startsWith("#include \"algovi_debug.h\""))
      {
         continue;
      }
      destFile.write(str.toLocal8Bit());
   }
   destFile.close();
   return QString();
}

inline QString processDebugStr(const QString& str)
{
   static QString lastColor;
   static int colorsCnt = 0;
   QString ret;
   if(!lastColor.isEmpty())
   {
      ret = "<font color=" + lastColor + ">";
   }
   const QString startcolor = "[color=";
   const QString endcolor = "[color]";
   for(int i = 0; i < str.length(); ++i)
   {
      bool check = i > 5;
      for(int j = i - 6; j <= i && check; ++j)
      {
         check &= startcolor[j - i + 6] == str[j];
      }
      if(check)
      {
         ret.chop(6);
         ++i;
         lastColor.clear();
         while(i < str.length() && str[i] != ']')
         {
            lastColor += str[i];
            ++i;
         }
         ret += "<font color=" + lastColor + ">";
         ++colorsCnt;
      }
      else
      {
         check = i > 5;
         for(int j = i - 6; j <= i && check; ++j)
         {
            check &= endcolor[j - i + 6] == str[j];
         }
         if(check)
         {
            ret.chop(6);
            if(colorsCnt > 0)
            {
               --colorsCnt;
               ret += "</font>";
            }
            lastColor.clear();
         }
         else
         {
            ret += NCommon::convertToHtml(str[i]);
         }
      }
   }
   if(!lastColor.isEmpty())
   {
      ret += "</font>";
   }
   return ret;
}

inline QString clearCodePath(const QString& path, QString& dest)
{
   QString folder = NFileSystem::get_file_path(path);
   QString name = NFileSystem::get_file_name(path);
   dest = folder + "/" + name + "_clear." + NFileSystem::get_file_extension(path);
   return preprocessCode(path, dest);
}

}
