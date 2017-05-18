/**
 * Project   AlgoVi
 *
 * @file     CSettingsReader.hpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#pragma once

#include <QString>
#include <QMap>

#include "framework/common/ClassGenerators.hpp"

namespace NSettings
{

typedef QMap <QString, QString> tAttributeMap;

class CSettingsData
{
ADD_ATTRIBUTE(CSettingsData, Name, QString)
ADD_ATTRIBUTE(CSettingsData, Attributes, tAttributeMap)
ADD_ATTRIBUTE(CSettingsData, Value, QString)
};

class CSettingsReader
{
public: // methods
    CSettingsReader(const QString& configPath);
    QVector <CSettingsData> readConfig();
    virtual bool fillSettings() = 0;
private: // fields
    QString mConfigPath;
};

} // namespace NSettings

