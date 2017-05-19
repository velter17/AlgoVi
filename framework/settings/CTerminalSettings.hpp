/**
 * Project   AlgoVi
 *
 * @file     CTerminalSettings.hpp
 * @author   Dmytro Sadovyi
 * @date     19.05.2017
 */

#pragma once

#include <QVector>
#include <QString>

#include "framework/common/ClassGenerators.hpp"

namespace NSettings
{

typedef QPair<QString, QString> tColor;
typedef QMap <QString, QString> tColorContainer;

class CTerminalSettings
{
SINGLETON_CLASS_BASE(CTerminalSettings)
ADD_ATTRIBUTE(CTerminalSettings, HomeDir, QString)
ADD_ATTRIBUTE(CTerminalSettings, HistorySize, int)
ADD_ATTRIBUTE(CTerminalSettings, Font, QString)
ADD_ATTRIBUTE(CTerminalSettings, FontSize, int)
ADD_ATTRIBUTE_WITH_REF(CTerminalSettings, Colors, tColorContainer)
private:
    CTerminalSettings()
    {
        tColorContainer container;
        container["background"] = "#000000";
        container["main"]       = "#00FF00";
        container["output"]     = "#FFFFFF";
        container["error"]      = "#FF3333";
        setColors(container);
        setHomeDir("$HOME");
        setFont("monospace");
        setFontSize(12);
        setHistorySize(500);
    }
};

} // namespace NSettings

