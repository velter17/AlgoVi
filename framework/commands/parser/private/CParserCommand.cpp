/**
 * Project   AlgoVi
 *
 * @file     CParserCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     10.05.2017
 */

#include <QTimer>
#include <QtNetwork/QNetworkReply>

#include "framework/commands/CCompiler.hpp"
#include "framework/commands/parser/CParserCommand.hpp"
#include "framework/commands/ProcessHelper.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace
{

#define NETWORK_ERROR(val) {QNetworkReply::NetworkError::val, #val}

QMap<QNetworkReply::NetworkError, QString> getNetworkErrors()
{
   return QMap<QNetworkReply::NetworkError, QString> {
      NETWORK_ERROR(NoError),
      NETWORK_ERROR(ConnectionRefusedError),
      NETWORK_ERROR(RemoteHostClosedError),
      NETWORK_ERROR(HostNotFoundError),
      NETWORK_ERROR(TimeoutError),
      NETWORK_ERROR(OperationCanceledError),
      NETWORK_ERROR(SslHandshakeFailedError),
      NETWORK_ERROR(TemporaryNetworkFailureError),
      NETWORK_ERROR(NetworkSessionFailedError),
      NETWORK_ERROR(BackgroundRequestNotAllowedError),
      NETWORK_ERROR(UnknownNetworkError),

      NETWORK_ERROR(ProxyConnectionRefusedError),
      NETWORK_ERROR(ProxyConnectionClosedError),
      NETWORK_ERROR(ProxyNotFoundError),
      NETWORK_ERROR(ProxyTimeoutError),
      NETWORK_ERROR(ProxyAuthenticationRequiredError),
      NETWORK_ERROR(UnknownProxyError),

      NETWORK_ERROR(ContentAccessDenied),
      NETWORK_ERROR(ContentOperationNotPermittedError),
      NETWORK_ERROR(ContentNotFoundError),
      NETWORK_ERROR(AuthenticationRequiredError),
      NETWORK_ERROR(ContentReSendError),
      NETWORK_ERROR(UnknownContentError),

      NETWORK_ERROR(ProtocolUnknownError),
      NETWORK_ERROR(ProtocolInvalidOperationError),
      NETWORK_ERROR(ProtocolFailure)
   };
}

} // anonymous namespace

namespace NCommand
{

CParserCommand::CParserCommand()
   : mTerminated(false)
   , mProc(nullptr)
{
   mOptions.add_options()
      ("url,u", boost::program_options::value <std::string>()->required(),
         "url of web-site with tests")
      ("parser,p", boost::program_options::value<std::string>(),
         "path to code of parser");

   connect(&mManager, &QNetworkAccessManager::finished, [this](QNetworkReply* reply){
      qDebug () << "some reply finished";
      parseTests(reply->readAll());
   });

   mDir.setAutoRemove(true);
}

void CParserCommand::run()
{
   emit started();
   boost::program_options::variables_map varMap;
   if(!readOptions(mArgs, varMap))
   {
       emit finished(1);
       return;
   }
   mUrl = QString::fromStdString(varMap["url"].as<std::string>());
   mParserPath = NFileSystem::get_full_path(
            QString::fromStdString(varMap["parser"].as<std::string>()));
   compileParser();
}

void CParserCommand::appendData(const QString& str)
{

}

void CParserCommand::setArgs(const QStringList& args)
{
   mArgs = args;
}

void CParserCommand::terminate()
{
   qDebug () << "CParserCommand::terminate()";
   mTerminated = true;
   if(mProc != nullptr)
   {
      mProc->terminate();
   }
}

void CParserCommand::readUrl(int tries)
{
   if(tries > 4 || mTerminated)
   {
      emit log(" [ Error ] there is no reply after " + QString::number(tries + 1) + " tries\n");
      emit finished(1);
   }
   else
   {
      qDebug () << "try #" << tries + 1;
      QUrl url = mUrl;
      QNetworkRequest request(url);
      QNetworkReply* reply(mManager.get(request));
      reply->ignoreSslErrors();

      QTimer::singleShot(5000, reply, SLOT(abort()));

      connect(reply, static_cast<void(QNetworkReply::*)(QNetworkReply::NetworkError)>(&QNetworkReply::error),
              [this, tries](QNetworkReply::NetworkError error){
         static QMap<QNetworkReply::NetworkError, QString> errorMap = getNetworkErrors();
         emit log (" [ ! ] Error after " + QString::number(tries+1) + " try: " + errorMap[error] + "\n");
         readUrl(tries+1);
      });
   }
}

void CParserCommand::compileParser()
{
   QString extension = NFileSystem::get_file_extension(mParserPath);
   ProgLanguage::EType lang = getProgLanguageType(extension.toStdString());
   CCompiler* compiler = new CCompiler({CCompiler::SCompilerTask(mParserPath, QStringList(), lang)});
   connect(compiler, &CCompiler::log, [this](const QString& msg){
       emit log(msg);
   });
   connect(compiler, &CCompiler::error, [this](const QString& msg){
       emit error(msg);
   });
   connect(compiler, &CCompiler::finished, [this, compiler](int code){
       if(code == 0)
       {
           mParserPath = compiler->getResult().front();
           readUrl(0);
       }
       else
       {
           emit finished(1);
       }
       compiler->deleteLater();
   });
   compiler->run();
}

void CParserCommand::parseTests(const QString& htmlContent)
{
    if(mTerminated)
    {
       emit log( " [ Terminated ]\n");
       emit finished(1);
       return;
    }
    mProc = new QProcess();
    QObject::connect(mProc, static_cast<void(QProcess::*)(int)>(&QProcess::finished), [this](int code){
       if(code == 0 && !mTerminated)
       {
          readTests();
       }
       else
       {
          if(!mTerminated)
          {
             emit log(" [ Error ] parser returned error code\n");
          }
          emit finished(1);
       }
       mProc->deleteLater();
       mProc = nullptr;
    });
    QObject::connect(mProc, static_cast<void(QProcess::*)(QProcess::ProcessError)>(&QProcess::error),
                               [this](QProcess::ProcessError err){
        if(!mTerminated)
        {
            emit error(processErrorToStr(err) + "\n");
        }
        emit finished(1);
        mProc->deleteLater();
        mProc = nullptr;
    });
    QStringList args;
    QFile file(mDir.path() + "/page.html");
    file.open(QFile::WriteOnly);
    file.write(htmlContent.toLocal8Bit());
    args << (mDir.path() + "/tests.data") << file.fileName();
    mProc->start(mParserPath, args);
}

void CParserCommand::readTests()
{
   log(" [ Finished ] " + QString::number(-1) + " tests were read\n");
   QFile file(mDir.path() + "/tests.data");
   file.open(QFile::ReadOnly);
   QString fileData = file.readAll();
   qDebug () << "fileData: " << fileData;
   emit finished(0);
}

} // namespace NCommand
