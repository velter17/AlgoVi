/**
 * Project   AlgoVi
 *
 * @file     CPlainTextTerminal.hpp
 * @author   Dmytro Sadovyi
 * @date     30.03.2017
 */


#pragma once

#include <memory>
#include <QKeyEvent>
#include <QPalette>

#include "controller/terminal/CTerminalBase.hpp"

namespace NController
{

class CPlainTextTerminalImpl;

class CPlainTextTerminal : public CTerminalBase
{
   Q_OBJECT
public: // methods
    CPlainTextTerminal(QWidget* parent);

    void appendSimpleText(const QString& text) override;
    void appendHtmlText(const QString& text) override;
    void appendErrorText(const QString& text) override;

    QWidget* getWidget();

private: // methods
    void displaySimpleText(const QString& text) override;
    void displayHtmlText(const QString& text) override;
    void onWriterChanged(WriterType::EType newWriter);

    template <TerminalMode::EType mode>
    void keyPressHandler(QKeyEvent* e);

private slots:
    void tabKeyHandler();
    void keyPressAssigner(QKeyEvent* e);

private: // fields
    QPalette mPalette;
    QString mInputBuffer;
    int mTabPressCount;
    QStringList mHistory;
    QStringList::iterator mHistoryItr;
    QString mLastTypedCommand;
    std::shared_ptr<CPlainTextTerminalImpl> mWidget;
};

} // namespace NController

