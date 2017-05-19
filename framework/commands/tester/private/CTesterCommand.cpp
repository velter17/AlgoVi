/**
 * Project   AlgoVi
 *
 * @file     CTesterCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     12.05.2017
 */

#include <QDebug>

#include "framework/commands/CCompiler.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/commands/tester/CTesterCommand.hpp"
#include "framework/commands/tester/private/CTesterImpl.hpp"
#include "framework/filesystem/filesystem.hpp"
#include "framework/commands/testCommand/TableDrawer.hpp"
#include "framework/commands/executor/DebugHelper.hpp"
#include "framework/settings/CRunnerSettings.hpp"

namespace
{

bool maskContainChar(int mask, char c)
{
   return (mask & (1 << (c - 'a'))) != 0;
}

QString colorize(const QString& text, const QString& color)
{
    return "<font color=" + color + ">" + text + "</font>";
}

QString getResultColor(NCommand::TesterResult::EType type)
{
   static QMap<NCommand::TesterResult::EType, QString> mp
   {
      {NCommand::TesterResult::Accepted, "#009900"},
      {NCommand::TesterResult::WrongAnswer, "#ff0000"},
      {NCommand::TesterResult::CheckFailed, "#ff33c0"},
      {NCommand::TesterResult::PresentationError, "#ff33c0"},
      {NCommand::TesterResult::RuntimeError, "#ff33c0"},
      {NCommand::TesterResult::TimeLimitExceeded, "#ff33c0"},
      {NCommand::TesterResult::CompilationError, "#ff33c0"},
      {NCommand::TesterResult::Total, "#ff33c0"},
   };

   return mp[type];
}

QString getResultTitle(NCommand::TesterResult::EType type)
{
   static QMap <NCommand::TesterResult::EType, QString> mp
   {
      {NCommand::TesterResult::Accepted, "OK"},
      {NCommand::TesterResult::WrongAnswer, "WA"},
      {NCommand::TesterResult::CheckFailed, "CF"},
      {NCommand::TesterResult::PresentationError, "PE"},
      {NCommand::TesterResult::RuntimeError, "RE"},
      {NCommand::TesterResult::TimeLimitExceeded, "TLE"},
      {NCommand::TesterResult::CompilationError,  "CE"},
      {NCommand::TesterResult::Total, "??"},
   };

   return mp[type];
}

QString resultToStr(NCommand::TesterResult::EType type)
{
   return colorize(getResultTitle(type), getResultColor(type));
}

} // anonymous namespace

namespace NCommand
{

CTesterCommand::CTesterCommand()
    : mAcceptedTests(0)
    , mCheckedTests(0)
    , mTerminatedFlag(false)
{
   mOptions.add_options()
      ("src,s", boost::program_options::value<std::string>()->required(), "source code path")
      ("checker,c", boost::program_options::value<std::string>()->default_value("testlib_wcmp"),
         "checker code path\n"
         "[or name of builtin, see manual]")
      ("test,t", boost::program_options::value<std::string>(),
         "if no param - test whole test archive\n"
         "if a-b -> test on range [a..b]\n"
         "if :a,b,..,c -> test on list (a,b,..,c)\n")
      ("time-limit", boost::program_options::value<int>()->default_value(2000),
         "time limit [in ms]")
      ("info,i", boost::program_options::value<std::string>()->default_value(""),
         "info report flags, (example '--info lta')\n"
         "  a - all verdicts in list\n"
         "  c - checker reports\n"
         "  l - test list (without passed)\n"
         "  t - execution time\n")
      ("remove-debug", boost::program_options::bool_switch()->default_value(false),
         "execute without debug instructions")
      ("verbose", boost::program_options::bool_switch()->default_value(false),
         "detailed report");

   mOptionValues["--checker"].append({
                                      "testlib_hcmp",
                                      "testlib_icmp",
                                      "testlib_lcmp",
                                      "testlib_ncmp",
                                      "testlib_rcmp",
                                      "testlib_rcmp4",
                                      "testlib_rcmp6",
                                      "testlib_rcmp9",
                                      "testlib_uncmp",
                                      "testlib_wcmp",
                                      "testlib_yesno"
                                   });
}

void CTesterCommand::run()
{
   emit started();
   boost::program_options::variables_map varMap;
   if(!readOptions(mArgs, varMap))
   {
      emit finished(1);
      return;
   }

   mCodePath = NFileSystem::get_full_path(
            QString::fromStdString(varMap["src"].as<std::string>()));
   if(varMap["remove-debug"].as<bool>())
   {
      QString dst;
      QString msg = clearCodePath(mCodePath, dst);
      if(!msg.isEmpty())
      {
         emit error(" [ Error ] " + msg + "\n");
         emit finished(1);
         return;
      }
      else
      {
         mCodePath = dst;
      }
   }

   mCheckerPath = QString::fromStdString(varMap["checker"].as<std::string>());
   if(!mCheckerPath.startsWith("testlib_"))
   {
      mCustomChecker = true;
      mCheckerPath = NFileSystem::get_full_path(mCheckerPath);
   }
   else
   {
      mCustomChecker = false;
      mCheckerAppPath = NFileSystem::get_full_system_path("checkers/" + mCheckerPath);
   }

   mVerboseFlag = varMap["verbose"].as<bool>();

   mTimeLimit = varMap.count("time-limit")
            ? varMap["time-limit"].as<int>()
            : 0;

   mInfoMask = 0;
   for(char c : varMap["info"].as<std::string>())
   {
      const QString allowedLetters = "aclt";
      if(!allowedLetters.contains(QChar(c)))
      {
         emit error(" [ Error ] Wrong info mask, '" + QString(c) + "' is not allowed\n");
         emit finished(1);
         return;
      }
      mInfoMask ^= (1 << (c - 'a'));
   }

   std::function <void()> testFunc;
   if(varMap.count("test"))
   {
      QString tests = QString::fromStdString(varMap["test"].as<std::string>());
      if(tests[0] == ':')
      {
         tList list = parseList(tests);
         if(validateList(list, CTestProvider::getInstance().size()))
         {
            testFunc = std::bind(&CTesterCommand::testList, this, list, 0);
         }
         else
         {
            emit error(" [ Error ] wrong test list\n");
            emit finished(1);
            return;
         }
      }
      else
      {
         tRange range = parseRange(tests);
         if(validateRange(range, CTestProvider::getInstance().size()))
         {
            testFunc = std::bind(&CTesterCommand::testRange, this, range, range.first);
         }
         else
         {
            emit error(" [ Error ] invalid test range\n");
            emit finished(1);
            return;
         }
      }
   }
   else
   {
      testFunc = std::bind(&CTesterCommand::testRange, this,
                           tRange(1, CTestProvider::getInstance().size()), 1);
   }

   connect(this, &CTesterCommand::compilationFinished, [this, testFunc](int code){
      if(code != 0)
      {
         emit error(" [ Error ] problems with compilation\n");
         emit finished(1);
      }
      else
      {
         testFunc();
      }
   });

   compile();
}

void CTesterCommand::appendData(const QString& str)
{

}

void CTesterCommand::setArgs(const QStringList& args)
{
   mArgs = args;
}

void CTesterCommand::terminate()
{
   mTerminatedFlag = true;
   if(nullptr != mTesterImpl)
   {
      mTesterImpl->terminate();
   }
}

void CTesterCommand::compile()
{
   std::vector <CCompiler::SCompilerTask> tasks {
         CCompiler::SCompilerTask(
            mCodePath,
            QStringList(),
            NSettings::parseProgLanguage(mCodePath))
      };
   if(mCustomChecker)
   {
      tasks.push_back(CCompiler::SCompilerTask(
                         mCheckerPath,
                         QStringList(),
                         NSettings::parseProgLanguage(mCheckerPath)));
   }

   CCompiler* compiler = new CCompiler(tasks);

   connect(compiler, &CCompiler::finished, [this, compiler](int code){
      if(code == 0)
      {
          mAppPath = compiler->getResult()[0];
          if(mCustomChecker)
          {
              mCheckerAppPath = compiler->getResult()[1];
          }
      }
      compiler->deleteLater();
      emit compilationFinished(code);
   });

   connect(compiler, &CCompiler::log, [this](const QString& msg){
      emit log(msg);
   });
   connect(compiler, &CCompiler::error, [this](const QString& msg){
      emit error(msg);
   });

   compiler->run();
}

void CTesterCommand::testRange(tRange range, int test)
{
   if(test > range.second || mTerminatedFlag)
   {
       finish();
   }
   else
   {
       runTest(test, std::bind(&CTesterCommand::testRange, this, range, test+1));
   }
}

void CTesterCommand::testList(const tList& list, int idx)
{
   if(idx >= list.size() || mTerminatedFlag)
   {
       finish();
   }
   else
   {
       runTest(list[idx], std::bind(&CTesterCommand::testList, this, list, idx+1));
   }
}

void CTesterCommand::runTest(int idx, std::function<void()> callback)
{
   qDebug () << "runTest " << idx;
   mTesterImpl = new CTesterImpl(CTesterJob().setAppPath(mAppPath)
                                             .setCheckerPath(mCheckerAppPath)
                                             .setTestNumber(idx)
                                             .setVerbose(mVerboseFlag)
                                             .setCustomChecker(mCustomChecker)
                                             .setTimeLimit(mTimeLimit));
   connect(mTesterImpl, &CTesterImpl::finished, [this, callback, idx](const CTesterResult& result){
       if(result.getResult() == TesterResult::Accepted)
       {
           ++mAcceptedTests;
       }
       ++mCheckedTests;
       if(mVerboseFlag)
       {
          std::vector <std::vector <CCell> > table {
             {CCell().setData({QString::number(idx)}),
              CCell().setData({"input"}).setAlign(TAlign::Center).setColor("#BFA8FF"),
              CCell().setData({"output"}).setAlign(TAlign::Center).setColor("#FF9999"),
              CCell().setData({"actual"}).setAlign(TAlign::Center).setColor("#00ffff")},
             {CCell().setData({QString::number(result.getExecutionTime()) + "ms"}),
              CCell().setData(CTestProvider::getInstance().getShortTest(idx-1).first.toVector().toStdVector()),
              CCell().setData(CTestProvider::getInstance().getShortTest(idx-1).second.toVector().toStdVector()),
              CCell().setData(result.getOutput().split('\n').toVector().toStdVector())},
             {CCell().setData({getResultTitle(result.getResult())})
                     .setColor(getResultColor(result.getResult()))
                     .setAlign(TAlign::Center),
              CCell().setData({"Checker: " + result.getMessage()}).setCollspan(3)}
          };

          emit logHtml(getHtmlTable(table, "#A0A0A0"));
       }
       else if(maskContainChar(mInfoMask, 'l'))
       {
           if(maskContainChar(mInfoMask, 'a') || result.getResult() != TesterResult::Accepted)
           {
              emit log(" test #" + QString::number(idx) + ": ");
              QString toLog = resultToStr(result.getResult());
              emit logHtml(toLog);
              if(maskContainChar(mInfoMask, 't'))
              {
                 toLog = toHtmlSymbol(SPACE) + "[ " + QString::number(result.getExecutionTime()) + "ms ]";
                 emit logHtml(colorize(toLog, "#33ffff"));
              }
              if(maskContainChar(mInfoMask, 'c'))
              {
                 toLog = " -> ( " + result.getMessage() + " )";
                 emit log(toLog);
              }
              emit log("\n");
           }
       }
       mTesterImpl->deleteLater();
       mTesterImpl = nullptr;
       callback();
   });

   mTesterImpl->execute();
}

void CTesterCommand::finish()
{
    std::vector <std::vector <CCell>> table {
       {CCell().setAlign(TAlign::Center).setCollspan(2).setData({"Test report"})},
       {CCell().setData({QString::number(mAcceptedTests) + " / " + QString::number(mCheckedTests)}),
        CCell().setData({mAcceptedTests == mCheckedTests ? "Passed" : "Partial solution"})
               .setColor(mAcceptedTests == mCheckedTests ? "#9FFF3F" : "#FF6F3F")}
    };
    emit logHtml(getHtmlTable(table));
    emit finished(0);
}


} // namespace NCommand
