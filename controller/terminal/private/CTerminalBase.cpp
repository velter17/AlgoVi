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
   : mLastWriter(WriterType::System)
   , mPrevMode(TerminalMode::WaitForCommand)
   , mMode(TerminalMode::WaitForCommand)
   , mPromptMessage("AlgoVi")
{

}

void CTerminalBase::lock()
{
    assert(mMode != TerminalMode::Locked);

    mPrevMode = mMode;
    mMode = TerminalMode::Locked;
}

void CTerminalBase::unlock()
{
   mMode = mPrevMode;
   displayNewCommandPrompt();
}

void CTerminalBase::setQuestionMode()
{
   mPrevMode = mMode;
   mMode = TerminalMode::Question;
}

void CTerminalBase::setInsideProcessMode()
{
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
