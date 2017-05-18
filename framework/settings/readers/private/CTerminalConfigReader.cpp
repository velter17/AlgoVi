/**
 * Project   AlgoVi
 *
 * @file     CTerminalConfigReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>

#include "framework/settings/readers/CTerminalConfigReader.hpp"

namespace NSettings
{

CTerminalConfigReader::CTerminalConfigReader(const QString& config)
   : CSettingsReader(config)
{

}

bool CTerminalConfigReader::fillSettings()
{
   QVector <CSettingsData> data = readConfig();
   for(const CSettingsData& item : data)
   {
      qDebug () << item.getName() << " -> " << item.getValue() << " [ " << item.getAttributes() << " ]";
   }
   return true;
}


} // namespace NSettings
