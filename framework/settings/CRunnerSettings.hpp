/**
 * Project   AlgoVi
 *
 * @file     CRunnerSettings.hpp
 * @author   Dmytro Sadovyi
 * @date     19.05.2017
 */

#pragma once

#include <QMap>

#include "framework/common/ClassGenerators.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NSettings
{

typedef QPair<QString, QString> tLanguage;
typedef QPair<QString, QString> tCompiler;
typedef QPair<QString, QString> tRunner;
typedef QMap <QString, QString> tContainer;

class CRunnerSettings
{
SINGLETON_CLASS_BASE_CONSTR(CRunnerSettings)
ADD_ATTRIBUTE_WITH_REF(CRunnerSettings, LangToExt, tContainer)
ADD_ATTRIBUTE_WITH_REF(CRunnerSettings, ExtToLang, tContainer)
ADD_ATTRIBUTE_WITH_REF(CRunnerSettings, Compilers, tContainer)
ADD_ATTRIBUTE_WITH_REF(CRunnerSettings, Runners, tContainer)
ADD_ATTRIBUTE_WITH_REF(CRunnerSettings, DebugCleaners, tContainer)
};

inline QString parseProgLanguage(const QString& filePath)
{
   QString extension = NFileSystem::get_file_extension(filePath);
   if(CRunnerSettings::getInstance().getExtToLang().find(extension) ==
         CRunnerSettings::getInstance().getExtToLang().end())
   {
      return "binary";
   }
   return CRunnerSettings::getInstance().getExtToLang()[extension];
}

} // namespace NSettings

