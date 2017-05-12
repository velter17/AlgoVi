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
#include <QTimer>
#include <QPlainTextEdit>
#include <QTextBlock>

#include "controller/terminal/CPlainTextTerminal.hpp"
#include "controller/terminal/TerminalHelpers.hpp"
#include "controller/terminal/private/CPlainTextTerminalImpl.hpp"

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
    : mWidget(new CPlainTextTerminalImpl(parent))
    , mTabPressCount(0)
{
    mHistoryItr = mHistory.end();

    mPalette.setColor(QPalette::Base, Colors::Background);
    mPalette.setColor(QPalette::Text, Colors::Main);
    mWidget->setPalette(mPalette);

    QFont font("monospace");
    font.setPixelSize(12);
    mWidget->setFont(font);

    connect(mWidget.get(), SIGNAL(keyPressed(QKeyEvent*)), this, SLOT(keyPressAssigner(QKeyEvent*)));

    displayNewCommandPrompt();
}

template <>
void CPlainTextTerminal::keyPressHandler<TerminalMode::WaitForCommand>(QKeyEvent* e)
{
    /* Single character */
    if(e->key() >= 0x20 && e->key() <= 0x7e
           && (e->modifiers() == Qt::NoModifier
               || e->modifiers() == Qt::ShiftModifier
               || e->modifiers() == Qt::KeypadModifier))
    {
        setWriter(WriterType::User);
        mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
        displayHtmlText(convertTextToHtml(e->text()));
        mInputBuffer += e->text();
    }

    /* ctrl + shift + v */
    if(e->key() == 0x56 && e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
    {
        setWriter(WriterType::User);
        mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
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
        mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
        mInputBuffer.chop(1);
        mWidget->pressKeyDefault(e);
    }

    /* Tab complation */
    if(e->key() == Qt::Key_Tab)
    {
       setWriter(WriterType::User);
       if(mTabPressCount == 0)
       {
          QTimer::singleShot(150, this, SLOT(tabKeyHandler()));
       }
       ++mTabPressCount;
    }

    /* Restore command from history */
    if(e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
    {
        QString toDisplay;
        if(e->key() == Qt::Key_Up && mHistoryItr == mHistory.end() && !mHistory.isEmpty())
        {
            mLastTypedCommand = mInputBuffer;
        }
        if(e->key() == Qt::Key_Up && !mHistory.isEmpty())
        {
            if(mHistoryItr != mHistory.begin())
            {
                --mHistoryItr;
            }
            toDisplay = *mHistoryItr;
        }
        if(e->key() == Qt::Key_Down && mHistoryItr == mHistory.end())
        {
            toDisplay = mInputBuffer;
        }
        if(e->key() == Qt::Key_Down && mHistoryItr != mHistory.end() && !mHistory.isEmpty())
        {
            ++mHistoryItr;
            if(mHistoryItr == mHistory.end())
            {
                toDisplay = mLastTypedCommand;
            }
            else
            {
                toDisplay = *mHistoryItr;
            }
        }
        QTextBlock block = mWidget->document()->lastBlock();
        QTextCursor p(block);
        p.movePosition(QTextCursor::EndOfBlock);
        p.setPosition(p.position()-mInputBuffer.size());
        p.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        p.removeSelectedText();
        displayHtmlText(toDisplay);
        mInputBuffer = toDisplay;
        p.movePosition(QTextCursor::EndOfBlock);
    }

    /* enter */
    if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
       (e->modifiers() == Qt::NoModifier ||
       e->modifiers() == Qt::KeypadModifier))
    {
       qDebug () << "CPlainTextTerminal::keyPressEvent() enter pressed, execute: " << mInputBuffer;
       if(mHistoryItr != mHistory.end() && *mHistoryItr == mInputBuffer)
       {
           mHistory.erase(mHistoryItr);
       }
       if(!mInputBuffer.isEmpty())
       {
           mHistory.append(mInputBuffer);
       }
       mHistoryItr = mHistory.end();
       mLastTypedCommand.clear();

       QString toEmit = mInputBuffer;
       mInputBuffer.clear();
       emit command(toEmit);
    }
}

template <>
void CPlainTextTerminal::keyPressHandler<TerminalMode::InsideProcess>(QKeyEvent *e)
{
    if(e->key() == Qt::Key_C && e->modifiers() == Qt::ControlModifier)
    {
        setWriter(WriterType::System);
        displayHtmlText("<font color=yellow>^C</font>");
        emit termination();
        return;
    }

   /* Single character */
   if(e->key() >= 0x20 && e->key() <= 0x7e
          && (e->modifiers() == Qt::NoModifier
              || e->modifiers() == Qt::ShiftModifier
              || e->modifiers() == Qt::KeypadModifier))
   {
       setWriter(WriterType::User);
       mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
       displayHtmlText(convertTextToHtml(e->text()));
       mInputBuffer += e->text();
   }

   /* ctrl + shift + v */
   if(e->key() == 0x56 && e->modifiers() == (Qt::ShiftModifier | Qt::ControlModifier))
   {
       setWriter(WriterType::User);
       mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
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
       mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
       mInputBuffer.chop(1);
       mWidget->pressKeyDefault(e);
   }

   /* enter + ctrl */
   if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
       e->modifiers() == Qt::ControlModifier)
   {
      mInputBuffer += "\n";
      displayHtmlText("<br>");
      mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
   }

   /* enter */
   if((e->key() == Qt::Key_Enter || e->key() == Qt::Key_Return) &&
      (e->modifiers() == Qt::NoModifier ||
      e->modifiers() == Qt::KeypadModifier))
   {
       qDebug () << "CPlainTextTerminal::keyPressEvent() enter pressed, newData: " << mInputBuffer;
       emit newData(mInputBuffer);
       mWidget->textCursor().insertBlock();
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

void CPlainTextTerminal::keyPressAssigner(QKeyEvent *e)
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

void CPlainTextTerminal::tabKeyHandler()
{
   int lastWordLen = 0;
   QStringList hints = complation(mInputBuffer, lastWordLen);
   qDebug () << "hints: " << hints;
   if(hints.empty())
   {
      mTabPressCount = 0;
      return;
   }
   if(mTabPressCount > 1)
   {
      setWriter(WriterType::System);
      for(const QString& s : hints)
      {
         appendSimpleText(s + " ");
      }
      appendSimpleText("\n");
      displayNewCommandPrompt();
      displayHtmlText(mInputBuffer);
   }
   else
   {
      QString toAppend;
      if(hints.size() == 1)
      {
         toAppend = hints.first();
      }
      else
      {
         int lcp = calcLCP(hints);
         qDebug () << "lcp == " << lcp;
         if(lcp > 0)
         {
             toAppend = hints.first().mid(lastWordLen, lcp-lastWordLen);
         }
      }
      while(!mInputBuffer.isEmpty() && mInputBuffer.endsWith(' '))
      {
         qDebug () << "mInputBuffer has space: " << mInputBuffer;
         mInputBuffer.chop(1);
         mWidget->textCursor().deletePreviousChar();
      }
      mInputBuffer += toAppend;
      displayHtmlText(convertTextToHtml(toAppend));
   }
   mTabPressCount = 0;
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

QWidget *CPlainTextTerminal::getWidget()
{
   return mWidget.get();
}

void CPlainTextTerminal::displaySimpleText(const QString& text)
{
   displayHtmlText(colorize(convertTextToHtml(text), Colors::Main));
}

void CPlainTextTerminal::displayHtmlText(const QString& text)
{
   mWidget->textCursor().insertHtml(text);
}

void CPlainTextTerminal::onWriterChanged(WriterType::EType newWriter)
{
   if(newWriter == WriterType::System)
   {
       if(mWidget->textCursor().columnNumber() == 0)
       {
          mWidget->textCursor().deletePreviousChar();
       }
       mWidget->textCursor().insertBlock();
   }
   mWidget->verticalScrollBar()->setValue(mWidget->verticalScrollBar()->maximum());
}

} // namespace NController
