/**
 * Project   AlgoVi
 *
 * @file     ITerminal.hpp
 * @author   Dmytro Sadovyi
 * @date     27.03.2017
 */

#pragma once

#include <QObject>
#include <QString>

namespace NController
{

class ITerminal : public QObject
{
   Q_OBJECT
public slots:
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void setQuestionMode() = 0;
    virtual void setInsideProcessMode() = 0;

    virtual void appendSimpleText(const QString& text) = 0;
    virtual void appendHtmlText(const QString& text) = 0;
    virtual void appendErrorText(const QString& text) = 0;

signals:
    virtual void command(const QString& cmd);
};

} // namespace NController
