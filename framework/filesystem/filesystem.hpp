/**
 * Project   AlgoVi
 *
 * @file     filesystem.hpp
 * @author   Dmytro Sadovyi
 * @date     10.04.2017
 */

#pragma once

#include <QString>

namespace NFileSystem
{

QStringList content_list(const QString& dir);

QString get_current_dir();

bool change_dir(const QString& dir);

bool is_directory(const QString& dir);
bool is_file(const QString& dir);
bool exists(const QString& dir);

bool is_full_path(const QString& path);
QString get_full_path(const QString& path);

QString get_file_name(const QString& file);
QString get_file_extension(const QString& file);
QString get_file_path(const QString& file);

bool remove(const QString& file);
bool create_dir(const QString& dir);

} // namespace NFileSystem

