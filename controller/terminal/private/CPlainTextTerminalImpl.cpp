/**
 * Project   AlgoVi
 *
 * @file     CPlainTextTerminalImpl.cpp
 * @author   Dmytro Sadovyi
 * @date     05.05.2017
 */

#include "controller/terminal/private/CPlainTextTerminalImpl.hpp"

namespace NController
{

CPlainTextTerminalImpl::CPlainTextTerminalImpl(QWidget* parent)
   : QPlainTextEdit(parent)
{
}

void CPlainTextTerminalImpl::keyPressEvent(QKeyEvent* e)
{
   emit keyPressed(e);
}

void CPlainTextTerminalImpl::pressKeyDefault(QKeyEvent *e)
{
   QPlainTextEdit::keyPressEvent(e);
}

} // namespace NController
