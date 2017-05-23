/**
 * Project   AlgoVi
 *
 * @file     CWorkingDir.cpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#include <QProcessEnvironment>

#include "../CWorkingDir.hpp"
#include "framework/settings/CTerminalSettings.hpp"

namespace NFileSystem
{

CWorkingDir& CWorkingDir::getInstance()
{
    static CWorkingDir instance;
    return instance;
}

void CWorkingDir::setCurrentPath(const boost::filesystem::path &pth)
{
    mCurrentPath = pth;
}

boost::filesystem::path CWorkingDir::getCurrentPath()
{
    return mCurrentPath;
}

CWorkingDir::CWorkingDir()
{
   QString homeDir = NSettings::CTerminalSettings::getInstance().getHomeDir();
   if(homeDir[0] == '$')
   {
      homeDir = getenv(homeDir.mid(1).toLocal8Bit());
   }
   mCurrentPath = homeDir.toStdString();
}

} // namespace NFileSystem
