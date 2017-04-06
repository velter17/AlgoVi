/**
 * Project   AlgoVi
 *
 * @file     CTerminalBase.cpp
 * @author   Dmytro Sadovyi
 * @date     29.03.2017
 */

#include <QDebug>
#include <cassert>

#include "controller/terminal/CTerminalBase.hpp"
#include "controller/terminal/TerminalHelpers.hpp"

namespace NController
{

CTerminalBase::CTerminalBase()
   : mLastWriter(WriterType::User)
   , mPrevMode(TerminalMode::WaitForCommand)
   , mMode(TerminalMode::WaitForCommand)
   , mPromptMessage("AlgoVi")
{

}

void CTerminalBase::lock()
{
    assert(mMode != TerminalMode::Locked);
    qDebug () << "CTerminalBase::unlock()";
    mPrevMode = mMode;
    mMode = TerminalMode::Locked;
}

void CTerminalBase::unlock()
{
   qDebug () << "CTerminalBase::unlock()";
   mMode = mPrevMode;
   if(mMode != TerminalMode::WaitForCommand)
   {
      qDebug () << "something is going wrong....";
   }
   displayNewCommandPrompt();
}

void CTerminalBase::setQuestionMode()
{
   qDebug () << "CTerminalBase::setQuestionMode()";
   mPrevMode = mMode;
   mMode = TerminalMode::Question;
}

void CTerminalBase::setInsideProcessMode()
{
   qDebug () << "CTerminalBase::setInsideProcessMode()";
   mPrevMode = mMode;
   mMode = TerminalMode::InsideProcess;
   setWriter(WriterType::System);
}

void CTerminalBase::setWriter(WriterType::EType type)
{
   if(getWriter() != type)
   {
      onWriterChanged(type);
   }
   mLastWriter = type;
}

WriterType::EType CTerminalBase::getWriter()
{
   return mLastWriter;
}

void CTerminalBase::displayNewCommandPrompt()
{
   setWriter(WriterType::User);
   QString promptMessage = convertTextToHtml(mPromptMessage);
   promptMessage += "&nbsp;<b>" + toHtmlSymbol(8921) + "</b>&nbsp;";
   displayHtmlText(promptMessage);
   mPromptMessageLength = mPromptMessage.length() + 3;
}

} // namespace NController
