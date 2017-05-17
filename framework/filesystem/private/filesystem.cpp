/**
 * Project   AlgoVi
 *
 * @file     filesystem.cpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#include <QDebug>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

#include "framework/filesystem/CWorkingDir.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NFileSystem
{

namespace fs = boost::filesystem;


fs::path get_absolute_path(const QString& path)
{
    qDebug () << "get_absolute_path " << path;
    QString pathToUse = path.endsWith("/") ? path.mid(0, path.length()-1) : path;
    fs::path p = pathToUse.toStdString();
    fs::path ret = fs::absolute(p, CWorkingDir::getInstance().getCurrentPath());
    if(!fs::exists(ret))
    {
        qDebug () << QString::fromStdString(ret.string()) << " does not exist";
    }
    return ret;
}

QStringList content_list(const QString& dir)
{
    QStringList ret;
    fs::path current_path = get_absolute_path(dir);
    if(!boost::filesystem::exists(current_path))
    {
        qDebug () << QString::fromStdString(current_path.string()) << " doesn't exist";
    }
    boost::filesystem::directory_iterator end_itr;
    for(boost::filesystem::directory_iterator itr(current_path);
        itr != end_itr; ++itr)
    {
        const boost::filesystem::directory_entry& entry = *itr;
        if(entry.path().filename().c_str()[0] == '.')
        {
            continue;
        }
        ret.append(QString::fromStdString(entry.path().filename().generic_string()));
    }
    return ret;
}

QString get_current_dir()
{
    return QString::fromStdString(CWorkingDir::getInstance().getCurrentPath().string());
}

bool change_dir(const QString& dir)
{
    qDebug () << "change dir " << dir;
    fs::path absol = get_absolute_path(dir);
    if(!fs::exists(absol))
    {
        return false;
    }
    absol.normalize();
    qDebug () << "absolute path is " << QString::fromStdString(absol.string());
    CWorkingDir::getInstance().setCurrentPath(absol);
    return true;
}

bool is_directory(const QString& dir)
{
    boost::filesystem::path p = get_absolute_path(dir);
    return boost::filesystem::is_directory(p);
}

bool is_file(const QString& dir)
{
    boost::filesystem::path p = get_absolute_path(dir);
    return fs::is_regular_file(p);
}

bool exists(const QString& dir)
{
    return fs::exists(get_absolute_path(dir));
}

bool is_full_path(const QString& path)
{
    return fs::exists(fs::path(path.toStdString()));
}

QString get_full_path(const QString& path)
{
    return QString::fromStdString(get_absolute_path(path).string());
}

QString get_file_name(const QString& file)
{
    fs::path p = get_absolute_path(file);
    return QString::fromStdString(p.stem().string());
}

QString get_full_system_path(const QString& path)
{
   fs::path p = path.toStdString();
   fs::path ret = fs::absolute(p);
   if(!fs::exists(ret))
   {
       qDebug () << QString::fromStdString(ret.string()) << " does not exist";
   }
   return QString::fromStdString(ret.string());
}

QString get_full_file_name(const QString &file)
{
   fs::path p = get_absolute_path(file);
   return QString::fromStdString(p.filename().string());
}

QString get_file_extension(const QString& file)
{
    fs::path p = get_absolute_path(file);
    return QString::fromStdString(p.extension().string()).mid(1);
}

QString get_file_path(const QString &file)
{
   fs::path p = get_absolute_path(file);
   return QString::fromStdString(p.parent_path().string());
}

bool remove(const QString& file)
{
    return fs::remove(get_absolute_path(file));
}

bool create_dir(const QString& dir)
{
   return fs::create_directory(get_absolute_path(dir));
}

} // namespace NFileSystem
