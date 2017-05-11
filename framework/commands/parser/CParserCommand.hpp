/**
 * Project   AlgoVi
 *
 * @file     CParserCommand.hpp
 * @author   Dmytro Sadovyi
 * @date     10.05.2017
 */

#pragma once

#include <QProcess>
#include <QtNetwork/QNetworkAccessManager>
#include <QTemporaryDir>

#include "framework/commands/CTerminalCommandBase.hpp"

class QNetworkReply;

namespace NCommand
{

class CParserCommand : public CTerminalCommandBase
{
public: // methods
    CParserCommand();
    void run() override;
    void appendData(const QString& str) override;
    void setArgs(const QStringList& args) override;
    void terminate() override;

private slots: // methods
    void readUrl(int tries = 0);
    void compileParser();
    void parseTests(const QString& htmlContent);
    void readTests();

private: // fields
    QStringList mArgs;
    QProcess* mProc;
    QString mUrl;
    QString mParserPath;
    bool mTerminated;
    QNetworkAccessManager mManager;
    QTemporaryDir mDir;
};

} // namespace NCommand

