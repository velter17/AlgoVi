/**
 * Project   AlgoVi
 *
 * @file     CCommandConfigReader.hpp
 * @author   Dmytro Sadovyi
 * @date     18.05.2017
 */

#pragma once

#include "framework/settings/readers/CSettingsReader.hpp"

namespace NSettings
{

class CCommandConfigReader : public CSettingsReader
{
public: // methods
    CCommandConfigReader(const QString& config);

    bool fillSettings() override;
};

} // namespace NSettings

