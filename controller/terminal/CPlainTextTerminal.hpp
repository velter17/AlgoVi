/**
 * Project   AlgoVi
 *
 * @file     CPlainTextTerminal.hpp
 * @author   Dmytro Sadovyi
 * @date     30.03.2017
 */

#pragma once

#include <QPlainTextEdit>

#include "controller/terminal/CTerminalBase.hpp"

namespace NController
{

class CPlainTextTerminal : public CTerminalBase, public QPlainTextEdit
{
public: // methods
    CPlainTextTerminal(QWidget* parent);

    void keyPressEvent(QKeyEvent* e) override;
    void mousePressEvent(QMouseEvent *e) override;
    void mouseDoubleClickEvent(QMouseEvent *e) override;
    void contextMenuEvent(QContextMenuEvent *e) override;

private: // fields
    struct Colors
    {
        static QColor Background;
        static QColor Main;
        static QColor Output;
        static QColor Error;
    };
    QPalette mPalette;
};

} // namespace NController

