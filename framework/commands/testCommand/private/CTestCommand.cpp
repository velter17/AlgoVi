/**
 * Project   AlgoVi
 *
 * @file     CTestCommand.cpp
 * @author   Dmytro Sadovyi
 * @date     09.05.2017
 */

#include <QVector>

#include "framework/commands/testCommand/CTestCommand.hpp"
#include "framework/commands/testCommand/CTestProvider.hpp"
#include "framework/commands/testCommand/TableDrawer.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"

namespace NCommand
{

CTestCommand::CTestCommand()
{
   mOptions.add_options()
      ("print,p", boost::program_options::value<std::string>(), "print test");
}

void CTestCommand::run()
{
   emit started();
   if(!readOptions(mArgs, mVarMap))
   {
      emit finished(1);
      return;
   }

   if(mVarMap.count("print"))
   {
      print();
   }
   emit finished(0);
}

void CTestCommand::setArgs(const QStringList &args)
{
   mArgs = args;
}

void CTestCommand::print()
{
   QString toPrint = QString::fromStdString(mVarMap["print"].as<std::string>());
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
             .setColor("#dfb24f")
             .setData({"input"}),
      CCell().setAlign(TAlign::Center)
             .setColor("#00ffff")
             .setData({"output"})};

   const tTest& test = CTestProvider::getInstance().getTest(idx-1);
   std::vector <std::vector <CCell> > table { head };
   QStringList input = test.first.split('\n');
   QStringList output = test.second.split('\n');
   table.push_back(std::vector <CCell> {
                      CCell().setAlign(TAlign::Left)
                             .setData(input.toVector().toStdVector())
                             .setColor("#ffffff"),
                      CCell().setAlign(TAlign::Left)
                             .setColor("#ffffff")
                             .setData(output.toVector().toStdVector())
                   });
   emit logHtml(getHtmlTable(table));
}

} // namespace NCommand
