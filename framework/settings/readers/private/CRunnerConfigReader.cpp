/**
 * Project   AlgoVi
 *
 * @file     CRunnerConfigReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>

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
      qDebug () << item.getName() << " -> " << item.getValue() << " [ " << item.getAttributes() << " ]";
   }
   return true;
}


} // namespace NSettings
