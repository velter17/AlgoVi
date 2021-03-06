/**
 * Project   AlgoVi
 *
 * @file     CTestCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     09.05.2017
 */

#include <QDebug>
#include <QVector>

#include "framework/commands/testCommand/CTestCommand.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/commands/testCommand/TableDrawer.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"
#include "gui/CTestEditor.hpp"

namespace NCommand
{

class CTestCreator
{
public: // methods
   void appendInput(const QString& input);
   void appendOutput(const QString& output);
   tTest getTest();
   int mode();
   void setMode(int mode);
private: // fields
   QString mInput;
   QString mOutput;
   int mMode;
};

void CTestCreator::appendInput(const QString& input)
{
   mInput += input + "\n";
}

void CTestCreator::appendOutput(const QString& output)
{
   mOutput += output + "\n";
}

tTest CTestCreator::getTest()
{
   return tTest(mInput, mOutput);
}

int CTestCreator::mode()
{
   return mMode;
}

void CTestCreator::setMode(int mode)
{
   mMode = mode;
}

CTestCommand::CTestCommand()
{
   mOptions.add_options()
      ("print,p", boost::program_options::value<std::string>(), "print tests")
      ("clear,c", boost::program_options::bool_switch()->default_value(false),
         "clear test archive")
      ("delete,d", boost::program_options::value<std::string>(), "delete tests")
      ("create", boost::program_options::bool_switch()->default_value(false),
         "create test by typing")
      ("edit,e", boost::program_options::value <int>(),
         "edit test")
      ("win,w", boost::program_options::bool_switch()->default_value(false),
         "create/edit tests using separate window")
      ("swap", boost::program_options::value<std::string>(), "swap tests => a-b")
      ("move", boost::program_options::value<std::string>(), "move test a to b => a-b");

   mSettings.setRevCtrlLogic(true);
}

void CTestCommand::run()
{
   emit started();
   if(!readOptions(mArgs, mVarMap))
   {
      emit finished(1);
      return;
   }

   if(mVarMap["clear"].as<bool>())
   {
      CTestProvider::getInstance().clear();
      emit finished(0);
   }
   else if(mVarMap.count("delete"))
   {
      deleteTests();
      emit finished(0);
   }
   else if(mVarMap["create"].as<bool>())
   {
      createTest();
   }
   else if(mVarMap.count("edit"))
   {
      editTest();
   }
   else if(mVarMap.count("swap"))
   {
      swapTests();
      emit finished(0);
   }
   else if(mVarMap.count("move"))
   {
      moveTest();
      emit finished(0);
   }
   else
   {
      print();
      emit finished(0);
   }
}

void CTestCommand::appendData(const QString& str)
{
   if(0 != mTestCreator)
   {
      if(mTestCreator->mode() == 0)
      {
         mTestCreator->appendInput(str);
         emit log("Output:\n");
         mTestCreator->setMode(1);
      }
      else
      {
         mTestCreator->appendOutput(str);
         CTestProvider::getInstance().addTest(mTestCreator->getTest());
         emit log(" [ Info ] test was added [number " +
                  QString::number(CTestProvider::getInstance().size()) + "]\n");
         emit finished(0);
      }
   }
}

void CTestCommand::terminate()
{
   if(0 != mTestCreator)
   {
      mTestCreator.reset();
      emit finished(1);
   }
   else if(0 != mTestEditor)
   {
      mTestEditor->terminate();
   }
}

void CTestCommand::setArgs(const QStringList &args)
{
   mArgs = args;
}

void CTestCommand::print()
{
   if(CTestProvider::getInstance().empty() && !mVarMap.count("print"))
   {
      emit log("test archive is empty\n");
      return;
   }

   QString toPrint = mVarMap.count("print") ?
            QString::fromStdString(mVarMap["print"].as<std::string>()) :
            "1-" + QString::number(CTestProvider::getInstance().size());
   if(toPrint[0] == ':')
   {
      tList list = parseList(toPrint);
      if(validateList(list, CTestProvider::getInstance().size()))
      {
         printTests(list);
      }
      else
      {
         emit error("Invalid list to print\n");
      }
   }
   else
   {
      tRange range = parseRange(toPrint);
      if(validateRange(range, CTestProvider::getInstance().size()))
      {
         printTests(range);
      }
      else
      {
         emit error("Invalid range to print\n");
      }
   }
}

void CTestCommand::printTests(const tRange& range)
{
   for(int i = range.first; i <= range.second; ++i)
   {
      printTest(i);
   }
}

void CTestCommand::printTests(const tList &list)
{
   for(int idx : list)
   {
      printTest(idx);
   }
}

void CTestCommand::printTest(int idx)
{
   static std::vector <CCell> head = {
      CCell().setAlign(TAlign::Center)
             .setColor("#ffffff"),
      CCell().setAlign(TAlign::Center)
             .setColor("#BFA8FF")
             .setData({"input"}),
      CCell().setAlign(TAlign::Center)
             .setColor("#FF9999")
             .setData({"output"})};

   tShortTest testToPrint = CTestProvider::getInstance().getShortTest(idx-1);
   std::vector <std::vector <CCell> > table { head };
   table[0][0].setData({QString::number(idx)});
   const QStringList& input = testToPrint.first;
   const QStringList& output = testToPrint.second;
   table.push_back(std::vector <CCell> {
                      CCell(),
                      CCell().setAlign(TAlign::Left)
                             .setData(input.toVector().toStdVector())
                             .setColor("#ffffff"),
                      CCell().setAlign(TAlign::Left)
                             .setColor("#ffffff")
                             .setData(output.toVector().toStdVector())
                   });
   emit logHtml(getHtmlTable(table, "#A0A0A0"));
}

void CTestCommand::deleteTests()
{
   QString toDelete = QString::fromStdString(mVarMap["delete"].as<std::string>());
   if(toDelete[0] == ':')
   {
      tList list = parseList(toDelete);
      if(validateList(list, CTestProvider::getInstance().size()))
      {
         std::sort(list.begin(), list.end());
         for(int i = int(list.size())-1; i >= 0; --i)
         {
            CTestProvider::getInstance().erase(list[i]-1);
         }
      }
      else
      {
         emit error("Invalid list to delete\n");
      }
   }
   else
   {
      tRange range = parseRange(toDelete);
      if(validateRange(range, CTestProvider::getInstance().size()))
      {
         CTestProvider::getInstance().erase(range.first-1, range.second-1);
      }
      else
      {
         emit error("Invalid range to delete\n");
      }
   }
}

void CTestCommand::createTest()
{
   if(mVarMap["win"].as<bool>())
   {
      mTestEditor.reset(new NAlgoViGUI::CTestEditor(NAlgoViGUI::CTestEditorSettings().setReadOnly(false)));
      connect(mTestEditor.get(), &NAlgoViGUI::CTestEditor::canceled, [this](){
         qDebug() << "testEditor :: canceled";
         emit finished(1);
      });
      connect(mTestEditor.get(), &NAlgoViGUI::CTestEditor::ok, [this](const tTest& test){
         CTestProvider::getInstance().addTest(test);
         emit log(" [ Info ] test was added [number " +
                  QString::number(CTestProvider::getInstance().size()) + "]\n");
         mTestEditor->close();
         emit finished(0);
      });
      mTestEditor->show();
   }
   else
   {
      mTestCreator.reset(new CTestCreator());
      mTestCreator->setMode(0);
      emit log("Input:\n");
   }
}

void CTestCommand::swapTests()
{
   QString toSwap = QString::fromStdString(mVarMap["swap"].as<std::string>());
   tRange range = parseRange(toSwap);
   if(validateTests(range, CTestProvider::getInstance().size()))
   {
      CTestProvider::getInstance().swap(range.first - 1, range.second - 1);
   }
   else
   {
      emit log("invalid tests to swap\n");
   }
}

void CTestCommand::moveTest()
{
   QString toMove = QString::fromStdString(mVarMap["move"].as<std::string>());
   tRange range = parseRange(toMove);
   if(validateTests(range, CTestProvider::getInstance().size()))
   {
      CTestProvider::getInstance().move(range.first - 1, range.second - 1);
   }
   else
   {
      emit log("invalid positions to move\n");
   }
}

void CTestCommand::editTest()
{
   int testNum = mVarMap["edit"].as<int>();
   if(testNum < 1 || testNum > CTestProvider::getInstance().size())
   {
      emit error(" [ Error ] Wrong test number to edit\n");
      emit finished(1);
   }
   else
   {
      mTestEditor.reset(new NAlgoViGUI::CTestEditor(
                           NAlgoViGUI::CTestEditorSettings().setReadOnly(false)
                                                            .setTest(CTestProvider::getInstance().getTest(testNum-1))));
      connect(mTestEditor.get(), &NAlgoViGUI::CTestEditor::canceled, [this](){
         emit finished(1);
      });
      connect(mTestEditor.get(), &NAlgoViGUI::CTestEditor::ok, [this, testNum](const tTest& test){
         CTestProvider::getInstance().setTest(testNum-1, test);
         emit log(" [ Info ] test was changed\n");
         mTestEditor->close();
         emit finished(0);
      });
      mTestEditor->show();
   }
}

} // namespace NCommand
