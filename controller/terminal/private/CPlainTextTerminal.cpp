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
#include <QScrollBar>

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

template <>
void CPlainTextTerminal::keyPressHandler<TerminalMode::WaitForCommand>(QKeyEvent *e)
{
    /* Single character */
    if(e->key() >= 0x20 && e->key() <= 0x7e
           && (e->modifiers() == Qt::NoModifier
               || e->modifiers() == Qt::ShiftModifier
               || e->modifiers() == Qt::KeypadModifier))
    {
        setWriter(WriterType::User);
        this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
        displayHtmlText(convertTextToHtml(e->text()));
        mInputBuffer += e->text();
    }

    /* ctrl + shift + v */
    if(e->key() == 0x56 && e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
    {
        setWriter(WriterType::User);
        this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
        QClipboard *clipboard = QApplication::clipboard();
        displayHtmlText(convertTextToHtml(clipboard->text()));
        mInputBuffer += clipboard->text();
    }

    /* Delete one character by backspace key */
    if(e->key() == Qt::Key_Backspace
           && e->modifiers() == Qt::NoModifier
           && !mInputBuffer.isEmpty())
    {
        setWriter(WriterType::User);
        this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
        mInputBuffer.chop(1);
        QPlainTextEdit::keyPressEvent(e);
    }

    /* enter */
    if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
       (e->modifiers() == Qt::NoModifier ||
       e->modifiers() == Qt::KeypadModifier))
    {
       qDebug () << "CPlainTextTerminal::keyPressEvent() enter pressed, execute: " << mInputBuffer;
       QString toEmit = mInputBuffer;
       mInputBuffer.clear();
       emit command(toEmit);
    }
}

template <>
void CPlainTextTerminal::keyPressHandler<TerminalMode::InsideProcess>(QKeyEvent *e)
{
   /* Single character */
   if(e->key() >= 0x20 && e->key() <= 0x7e
          && (e->modifiers() == Qt::NoModifier
              || e->modifiers() == Qt::ShiftModifier
              || e->modifiers() == Qt::KeypadModifier))
   {
       setWriter(WriterType::User);
       this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
       displayHtmlText(convertTextToHtml(e->text()));
       mInputBuffer += e->text();
   }

   /* ctrl + shift + v */
   if(e->key() == 0x56 && e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
   {
       setWriter(WriterType::User);
       this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
       QClipboard *clipboard = QApplication::clipboard();
       displayHtmlText(convertTextToHtml(clipboard->text()));
       mInputBuffer += clipboard->text();
   }

   /* Delete one character by backspace key */
   if(e->key() == Qt::Key_Backspace
          && e->modifiers() == Qt::NoModifier
          && !mInputBuffer.isEmpty())
   {
       setWriter(WriterType::User);
       this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
       mInputBuffer.chop(1);
       QPlainTextEdit::keyPressEvent(e);
   }

   /* enter */
   if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
      (e->modifiers() == Qt::NoModifier ||
      e->modifiers() == Qt::KeypadModifier))
   {
       qDebug () << "CPlainTextTerminal::keyPressEvent() enter pressed, newData: " << mInputBuffer;
       emit newData(mInputBuffer);
       textCursor().insertBlock();
       mInputBuffer.clear();
   }
}

template <>
void CPlainTextTerminal::keyPressHandler<TerminalMode::Question>(QKeyEvent *e)
{

}

template <>
void CPlainTextTerminal::keyPressHandler<TerminalMode::Locked>(QKeyEvent *e)
{

}


void CPlainTextTerminal::keyPressEvent(QKeyEvent *e)
{
   if(mMode == TerminalMode::WaitForCommand)
   {
      keyPressHandler<TerminalMode::WaitForCommand>(e);
   }
   else if(mMode == TerminalMode::InsideProcess)
   {
      keyPressHandler<TerminalMode::InsideProcess>(e);
   }
   else if(mMode == TerminalMode::Question)
   {
      keyPressHandler<TerminalMode::Question>(e);
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
   setWriter(WriterType::System);
   displayHtmlText(colorize(convertTextToHtml(text), Colors::Output));
}

void CPlainTextTerminal::appendHtmlText(const QString& text)
{
   qDebug () << "CPlainTextTerminal::appendHtmlText() " << text;
   setWriter(WriterType::System);
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
   if(!mInputBuffer.isEmpty() && getWriter() == WriterType::System)
   {
      for(int i = 0; i < mInputBuffer.length(); ++i)
      {
         textCursor().deletePreviousChar();
      }
   }
   textCursor().insertHtml(text);
   if(!mInputBuffer.isEmpty() && getWriter() == WriterType::System)
   {
      setWriter(WriterType::User);
      displayHtmlText(mInputBuffer);
   }
}

void CPlainTextTerminal::onWriterChanged(WriterType::EType newWriter)
{
   if(textCursor().columnNumber() == 0)
   {
      textCursor().deletePreviousChar();
   }
   textCursor().insertBlock();
   this->verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

} // namespace NController
