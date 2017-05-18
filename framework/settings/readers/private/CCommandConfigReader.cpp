/**
 * Project   AlgoVi
 *
 * @file     CCommandConfigReader.cpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#include <QDebug>

#include "framework/settings/readers/CCommandConfigReader.hpp"

namespace NSettings
{

CCommandConfigReader::CCommandConfigReader(const QString& config)
   : CSettingsReader(config)
{

}

bool CCommandConfigReader::fillSettings()
{
   QVector <CSettingsData> data = readConfig();
   for(const CSettingsData& item : data)
   {
      qDebug () << item.getName() << " -> " << item.getValue() << " [ " << item.getAttributes() << " ]";
   }
   return true;
}


} // namespace NSettings
