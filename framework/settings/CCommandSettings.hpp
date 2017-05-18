/**
 * Project   AlgoVi
 *
 * @file     CCommandSettings.hpp
 * @author   Dmytro Sadovyi
 * @date     19.05.2017
 */

#pragma once

#include <QVector>
#include <QString>

#include "framework/common/ClassGenerators.hpp"

namespace NSettings
{

typedef QPair<QString, QString> tCommand;
typedef QMap <QString, QString> tCommandContainer;

class CCommandSettings
{
SINGLETON_CLASS_BASE_CONSTR(CCommandSettings)
ADD_ATTRIBUTE(CCommandSettings, Commands, tCommandContainer)
};

} // namespace NSettings

