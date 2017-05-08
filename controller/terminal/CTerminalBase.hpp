/**
 * Project   AlgoVi
 *
 * @file     CTerminalBase.hpp
 * @author   Dmytro Sadovyi
 * @date     29.03.2017
 */

#pragma once

#include "controller/terminal/ITerminal.hpp"

namespace NController
{

struct WriterType
{
   enum EType
   {
      System,
      User,
   };
};

struct TerminalMode
{
   enum EType
   {
      WaitForCommand,
      InsideProcess,
      Question,
      Locked,
   };
};

class CTerminalBase : public ITerminal
{
   //Q_OBJECT
public: // methods
    CTerminalBase();

    void lock() override;
    void unlock() override;
    void setQuestionMode() override;
    void setInsideProcessMode() override;

    void setComplation(std::shared_ptr<NCommand::CComplationProvider> provider) override;

protected: // virtual methods
    virtual void displaySimpleText(const QString& text) = 0;
    virtual void displayHtmlText(const QString& text) = 0;
    virtual void onWriterChanged(WriterType::EType newWriter) = 0;

protected: // methods
    void setWriter(WriterType::EType type);
    WriterType::EType getWriter();
    QStringList complation(const QString& cmd, int& lastWordLen);

    virtual void displayNewCommandPrompt();
protected: // fields
    WriterType::EType mLastWriter;
    TerminalMode::EType mPrevMode;
    TerminalMode::EType mMode;
    QString mPromptMessage;
    int mPromptMessageLength;
    std::shared_ptr <NCommand::CComplationProvider> mComplationProvider;
};

} // namespace NController

