/**
 * Project   AlgoVi
 *
 * @file     CRunnerConfigReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>

#include "framework/settings/CRunnerSettings.hpp"
#include "framework/settings/readers/CRunnerConfigReader.hpp"

namespace NSettings
{

CRunnerConfigReader::CRunnerConfigReader(const QString& config)
   : CSettingsReader(config)
{

}

bool CRunnerConfigReader::fillSettings()
{
   QVector <CSettingsData> data = readConfig();
   for(const CSettingsData& item : data)
   {
      if(item.getName() == "Language")
      {
         if(item.getAttributes().find("name") == item.getAttributes().end() ||
            item.getAttributes().find("extension") == item.getAttributes().end())
         {
            return false;
         }
         CRunnerSettings::getInstance().getLangToExtRef()[item.getAttributes()["name"]] =
               item.getAttributes()["extension"];
         CRunnerSettings::getInstance().getExtToLangRef()[item.getAttributes()["extension"]] =
               item.getAttributes()["name"];
      }
      else if(item.getName() == "Compiler")
      {
         if(item.getAttributes().find("name") == item.getAttributes().end())
         {
            return false;
         }
         CRunnerSettings::getInstance().getCompilersRef()[item.getAttributes()["name"]] =
               item.getValue();
      }
      else if(item.getName() == "Runner")
      {
         if(item.getAttributes().find("name") == item.getAttributes().end())
         {
            return false;
         }
         CRunnerSettings::getInstance().getRunnersRef()[item.getAttributes()["name"]] =
               item.getValue();
      }
      else if(item.getName() == "DebugCleaner")
      {
         if(item.getAttributes().find("name") == item.getAttributes().end())
         {
            return false;
         }
         CRunnerSettings::getInstance().getDebugCleanersRef()[item.getAttributes()["name"]] =
               item.getValue();
      }
   }
   return true;
}


} // namespace NSettings
