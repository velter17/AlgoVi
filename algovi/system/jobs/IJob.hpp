/**
 * Project   AlgoVi
 *
 * @file     IJob.hpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#pragma once

#include <QObject>
#include <QStringList>

namespace NAlgoVi
{

class IJob : public QObject
{
   Q_OBJECT
public:
    virtual ~IJob(){}
public slots:
    virtual void run(const QStringList& args) = 0;
    virtual void appendData(const QString& data) = 0;
    virtual void terminate() = 0;
    virtual QStringList getArguments(){return QStringList();}
signals:
    void started();
    void finished();
};

} // namespace NAlgoVi

