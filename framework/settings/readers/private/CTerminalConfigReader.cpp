/**
 * Project   AlgoVi
 *
 * @file     CTerminalConfigReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>

#include "framework/settings/readers/CTerminalConfigReader.hpp"
#include "framework/settings/CTerminalSettings.hpp"

namespace NSettings
{

CTerminalConfigReader::CTerminalConfigReader(const QString& config)
   : CSettingsReader(config)
{

}

bool CTerminalConfigReader::fillSettings()
{
   QVector <CSettingsData> data = readConfig();
   const CTerminalSettings& settings = CTerminalSettings::getInstance();
   for(const CSettingsData& item : data)
   {
      if(item.getName() == "Color")
      {
          if(item.getAttributes().find("name") == item.getAttributes().end())
          {
              return false;
          }
          CTerminalSettings::getInstance().getColorsRef()[item.getAttributes()["name"]] = item.getValue();
      }
      else if(item.getName() == "Font")
      {
          CTerminalSettings::getInstance().setFont(item.getValue());
          if(item.getAttributes().find("size") != item.getAttributes().end())
          {
              CTerminalSettings::getInstance().setFontSize(item.getAttributes()["size"].toInt());
          }
      }
      else if(item.getName() == "HistorySize")
      {
          CTerminalSettings::getInstance().setHistorySize(item.getValue().toInt());
      }
      else if(item.getName() == "HomeDir")
      {
          CTerminalSettings::getInstance().setHomeDir(item.getValue());
      }
   }
   return true;
}


} // namespace NSettings
