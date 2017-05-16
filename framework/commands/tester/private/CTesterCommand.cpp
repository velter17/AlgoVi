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

namespace
{

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
{
   mOptions.add_options()
      ("src,s", boost::program_options::value<std::string>()->required(), "source code path")
      ("checker,c", boost::program_options::value<std::string>(), "checker code path\n"
                                                                  "[or name of builtin, see manual]")
      ("test,t", boost::program_options::value<std::string>(),
         "if no param - test whole test archive\n"
         "if a-b -> test on range [a..b]\n"
         "if :a,b,..,c -> test on list (a,b,..,c)\n")
      ("remove-debug", boost::program_options::bool_switch()->default_value(false),
         "execute without debug instructions")
      ("verbose", boost::program_options::bool_switch()->default_value(false),
         "detailed report");
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

   if(varMap.count("checker"))
   {
      mCheckerPath = NFileSystem::get_full_path(
               QString::fromStdString(varMap["checker"].as<std::string>()));
   }

   mVerboseFlag = varMap["verbose"].as<bool>();

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

}

void CTesterCommand::compile()
{
   std::vector <CCompiler::SCompilerTask> tasks {
         CCompiler::SCompilerTask(
            mCodePath,
            QStringList(),
            getProgLanguageType(NFileSystem::get_file_extension(mCodePath).toStdString()))
      };
   if(!mCheckerPath.isEmpty())
   {
      tasks.push_back(CCompiler::SCompilerTask(
                         mCheckerPath,
                         QStringList(),
                         getProgLanguageType(NFileSystem::get_file_extension(mCheckerPath).toStdString())));
   }

   CCompiler* compiler = new CCompiler(tasks);

   connect(compiler, &CCompiler::finished, [this, compiler](int code){
      if(code == 0)
      {
          mAppPath = compiler->getResult()[0];
          if(!mCheckerPath.isEmpty())
          {
              mCheckerAppPath = compiler->getResult()[1];
          }
          mCustomChecker = !mCheckerPath.isEmpty();
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
   if(test > range.second)
   {
       finish(range.second - range.first + 1);
   }
   else
   {
       runTest(test, std::bind(&CTesterCommand::testRange, this, range, test+1));
   }
}

void CTesterCommand::testList(const tList& list, int idx)
{
   if(idx >= list.size())
   {
       finish(list.size());
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
                                             .setTimeLimit(1000000));
   connect(mTesterImpl, &CTesterImpl::finished, [this, callback, idx](const CTesterResult& result){
       if(result.getResult() == TesterResult::Accepted)
       {
           ++mAcceptedTests;
       }
       if(mVerboseFlag)
       {
          std::vector <std::vector <CCell> > table {
             {CCell().setData({QString::number(idx)}),
              CCell().setData({"input"}).setAlign(TAlign::Center).setColor("#BFA8FF"),
              CCell().setData({"output"}).setAlign(TAlign::Center).setColor("#FF9999"),
              CCell().setData({"actual"}).setAlign(TAlign::Center).setColor("#00ffff")},
             {CCell(),
              CCell().setData(CTestProvider::getInstance().getTest(idx-1).first.split('\n').toVector().toStdVector()),
              CCell().setData(CTestProvider::getInstance().getTest(idx-1).second.split('\n').toVector().toStdVector()),
              CCell().setData(result.getOutput().split('\n').toVector().toStdVector())},
             {CCell().setData({getResultTitle(result.getResult())})
                     .setColor(getResultColor(result.getResult()))
                     .setAlign(TAlign::Center),
              CCell().setData({"Checker: " + result.getMessage()}).setCollspan(3)}
          };

          emit logHtml(getHtmlTable(table, "#A0A0A0"));
       }
       else
       {
           emit log(" test #" + QString::number(idx) + ": ");
           QString toLog = resultToStr(result.getResult());
           emit logHtml(toLog);
           // TODO: execution time
           toLog = " -> ( " + result.getMessage() + " )\n";
           emit log(toLog);
       }
       mTesterImpl->deleteLater();
       callback();
   });

   mTesterImpl->execute();
}

void CTesterCommand::finish(int tests)
{
    std::vector <std::vector <CCell>> table {
       {CCell().setAlign(TAlign::Center).setCollspan(2).setData({"Test report"})},
       {CCell().setData({QString::number(mAcceptedTests) + " / " + QString::number(tests)}),
        CCell().setData({mAcceptedTests == tests ? "Passed" : "Partial solution"})
               .setColor(mAcceptedTests == tests ? "#9FFF3F" : "#FF6F3F")}
    };
    emit logHtml(getHtmlTable(table));
    emit finished(0);
}


} // namespace NCommand
