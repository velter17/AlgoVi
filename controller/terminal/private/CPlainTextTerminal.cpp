/**
 * Project   AlgoVi
 *
 * @file     CPlainTextTerminal.cpp
 * @author   Dmytro Sadovyi
 * @date     30.03.2017
 */

#include <QApplication>
#include <QClipboard>

#include "controller/terminal/CPlainTextTerminal.hpp"

namespace NController
{

QColor CPlainTextTerminal::Colors::Background =
        QColor(Qt::black);
QColor CPlainTextTerminal::Colors::Main =
        QColor(Qt::green);
QColor CPlainTextTerminal::Colors::Error =
        QColor(Qt::red);
QColor CPlainTextTerminal::Colors::Output =
        QColor(Qt::white);

CPlainTextTerminal::CPlainTextTerminal(QWidget* parent)
    : QPlainTextEdit(parent)
{
    mPalette.setColor(QPalette::Base, Colors::Background);
    mPalette.setColor(QPalette::Text, Colors::Main);
    setPalette(mPalette);

    QFont font("monospace");
    font.setPixelSize(12);
    setFont(font);
}

void CPlainTextTerminal::keyPressEvent(QKeyEvent *e)
{
    /* Single character */
    if(e->key() >= 0x20 && e->key() <= 0x7e
           && (e->modifiers() == Qt::NoModifier
               || e->modifiers() == Qt::ShiftModifier
               || e->modifiers() == Qt::KeypadModifier))
                //QPlainTextEdit::keyPressEvent(e);
    {
        this->textCursor().insertText(e->text());
    }
    if(e->key() == 0x56 && e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
    {
        QClipboard *clipboard = QApplication::clipboard();
        this->textCursor().insertText(clipboard->text());
    }

    /* Delete one character by backspace key */
    if(e->key() == Qt::Key_Backspace
           && e->modifiers() == Qt::NoModifier)
    {
        QPlainTextEdit::keyPressEvent(e);
    }
}

void CPlainTextTerminal::mousePressEvent(QMouseEvent *e)
{

}

void CPlainTextTerminal::mouseDoubleClickEvent(QMouseEvent *e)
{

}

void CPlainTextTerminal::contextMenuEvent(QContextMenuEvent *e)
{

}

} // namespace NController
