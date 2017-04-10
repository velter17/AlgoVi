/**
 * Project   AlgoVi
 *
 * @file     CWorkingDir.hpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#pragma once

#include <boost/filesystem/path.hpp>

namespace NFileSystem
{

class CWorkingDir
{
public: // methods
    static CWorkingDir& getInstance();
    void setCurrentPath(const boost::filesystem::path& pth);
    boost::filesystem::path getCurrentPath();

private: // methods
    CWorkingDir();
    CWorkingDir(const CWorkingDir&);

private: // fields
    boost::filesystem::path mCurrentPath;
};

} // namespace NFileSystem

