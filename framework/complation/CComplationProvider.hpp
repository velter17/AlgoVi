/**
 * Project   AlgoVi
 *
 * @file     CComplationProvider.hpp
 * @author   Dmytro Sadovyi
 * @date     05.05.2017
 */

#pragma once

#include <QStringList>
#include <QMap>

namespace NCommand
{

typedef QMap<QString, QStringList> tArgumentsContainer;

class CComplationProvider
{
public: // methods
    CComplationProvider();
    void addCommand(const QString& name);
    void addCommand(const QString& name, const tArgumentsContainer& args);
    QStringList complation(const QString& cmd, int& lastWordLen);
private: // fields
    QStringList mCommandList;
    QMap <QString, tArgumentsContainer> mArgumentsList;
};

} // namespace NCommand

