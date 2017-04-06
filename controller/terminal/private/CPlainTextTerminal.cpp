/**
 * Project   AlgoVi
 *
 * @file     CPlainTextTerminal.cpp
 * @author   Dmytro Sadovyi
 * @date     30.03.2017
 */

#include <QApplication>
#include <QClipboard>
#include <QDebug>

#include "controller/terminal/CPlainTextTerminal.hpp"
#include "controller/terminal/TerminalHelpers.hpp"

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

    displayNewCommandPrompt();
}

void CPlainTextTerminal::keyPressEvent(QKeyEvent *e)
{
    /* Single character */
    if(e->key() >= 0x20 && e->key() <= 0x7e
           && (e->modifiers() == Qt::NoModifier
               || e->modifiers() == Qt::ShiftModifier
               || e->modifiers() == Qt::KeypadModifier))
    {
        this->textCursor().insertText(e->text());
        mInputBuffer += e->text();
    }

    /* ctrl + shift + v */
    if(e->key() == 0x56 && e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
    {
        QClipboard *clipboard = QApplication::clipboard();
        this->textCursor().insertText(clipboard->text());
        mInputBuffer += clipboard->text();
    }

    /* Delete one character by backspace key */
    if(e->key() == Qt::Key_Backspace
           && e->modifiers() == Qt::NoModifier
           && mPromptMessageLength < textCursor().positionInBlock())
    {
        mInputBuffer.chop(1);
        QPlainTextEdit::keyPressEvent(e);
    }

    /* enter */
    if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
       (e->modifiers() == Qt::NoModifier ||
       e->modifiers() == Qt::KeypadModifier))
    {
       qDebug () << "CPlainTextTerminal::keyPressEvent() enter pressed, execute: " << mInputBuffer;
       emit command(mInputBuffer);
       mInputBuffer.clear();
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

void CPlainTextTerminal::appendSimpleText(const QString& text)
{
   qDebug () << "CPlainTextTerminal::appendSimpleText() " << text;
   displayHtmlText(colorize(convertTextToHtml(text), Colors::Output));
}

void CPlainTextTerminal::appendHtmlText(const QString& text)
{
   qDebug () << "CPlainTextTerminal::appendHtmlText() " << text;
   displayHtmlText(text);
}

void CPlainTextTerminal::appendErrorText(const QString& text)
{
   displayHtmlText(colorize(convertTextToHtml(text), Colors::Error));
}

void CPlainTextTerminal::displaySimpleText(const QString& text)
{
   displayHtmlText(colorize(convertTextToHtml(text), Colors::Main));
}

void CPlainTextTerminal::displayHtmlText(const QString& text)
{
   textCursor().insertHtml(text);
}

void CPlainTextTerminal::onWriterChanged(WriterType::EType newWriter)
{
   if(textCursor().positionInBlock() > 0)
   {
      textCursor().insertBlock();
   }
}

} // namespace NController
