/**
 * Project   AlgoVi
 *
 * @file     CPlainTextTerminalImpl.hpp
 * @author   Dmytro Sadovyi
 * @date     05.05.2017
 */

#pragma once

#include <QPlainTextEdit>

namespace NController
{

class CPlainTextTerminalImpl : public QPlainTextEdit
{
   Q_OBJECT
public: // methods
   CPlainTextTerminalImpl(QWidget* parent);

   void keyPressEvent(QKeyEvent* e) override;
   void mousePressEvent(QMouseEvent *e) override;
   void mouseDoubleClickEvent(QMouseEvent *e) override;
   void contextMenuEvent(QContextMenuEvent *e) override;

   void pressKeyDefault(QKeyEvent* e);
signals:
   void keyPressed(QKeyEvent* e);
};

} // namespace NController

