/**
 * Project   AlgoVi
 *
 * @file     CWorkingDir.cpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#include "../CWorkingDir.hpp"

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
    : mCurrentPath("/home/dsadovyi")
{

}

} // namespace NFileSystem
