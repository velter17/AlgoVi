/**
 * Project   AlgoVi
 *
 * @file     CTerminalConfigReader.hpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#pragma once

#include "framework/settings/readers/CSettingsReader.hpp"

namespace NSettings
{

class CTerminalConfigReader : public CSettingsReader
{
public: // methods
    CTerminalConfigReader(const QString& config);

    bool fillSettings() override;
};

} // namespace NSettings

