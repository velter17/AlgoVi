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
#include "framework/common/HtmlConverter.hpp"
#include "framework/filesystem/filesystem.hpp"

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

void CTerminalBase::setComplation(std::shared_ptr<NCommand::CComplationProvider> provider)
{
   mComplationProvider = provider;
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

QStringList CTerminalBase::complation(const QString& cmd, int& lastWordLen)
{
   if(0 == mComplationProvider)
   {
      return QStringList();
   }
   return mComplationProvider->complation(cmd, lastWordLen);
}

void CTerminalBase::displayNewCommandPrompt()
{
   using namespace NCommon;
   setWriter(WriterType::User);
   QString titleColor = "#ffb266";
   QString promptMessage = colorize(convertToHtml(mPromptMessage), titleColor);
   promptMessage += colorize("<b>" + toHtmlSymbol(8867) + toHtmlSymbol(8866) + "</b>", "#ffffff");
   promptMessage += NFileSystem::get_current_dir();
   promptMessage += colorize("&nbsp;<b>" + toHtmlSymbol(8921) + "</b>&nbsp;", "#99ffff");
   displayHtmlText(promptMessage);
   mPromptMessageLength = mPromptMessage.length() + 3;
}

} // namespace NController
