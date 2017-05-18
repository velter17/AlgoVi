/**
 * Project   AlgoVi
 *
 * @file     CCommandConfigReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>

#include "framework/settings/readers/CCommandConfigReader.hpp"
#include "framework/settings/CCommandSettings.hpp"

namespace NSettings
{

CCommandConfigReader::CCommandConfigReader(const QString& config)
   : CSettingsReader(config)
{

}

bool CCommandConfigReader::fillSettings()
{
   QVector <CSettingsData> data = readConfig();
   tCommandContainer commands;
   for(const CSettingsData& item : data)
   {
      qDebug () << item.getName() << " -> " << item.getValue() << " [ " << item.getAttributes() << " ]";
      if(item.getName() != "Command" || item.getAttributes().find("name") == item.getAttributes().end())
      {
          return false;
      }
      commands.insert(item.getAttributes()["name"], item.getValue());
   }
   CCommandSettings::getInstance().setCommands(commands);
   return true;
}


} // namespace NSettings
