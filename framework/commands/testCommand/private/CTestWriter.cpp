/**
 * Project   AlgoVi
 *
 * @file     CTestWriter.cpp
 * @author   Dmytro Sadovyi
 * @date     14.05.2017
 */

#include <QDebug>
#include <QFile>

#include "framework/commands/testCommand/CTestWriter.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

CTestWriter::CTestWriter()
   : mTerminatedFlag(false)
   , mTestsWritten(0)
{
   mOptions.add_options()
      ("folder,f", boost::program_options::value<std::string>()->required(),
         "destination folder")
      ("test,t", boost::program_options::value<std::string>(),
         "tests for writing")
      ("pattern", boost::program_options::value<std::string>()->default_value("%1.(dat|ans)"),
         "file names pattern")
      ("num-length", boost::program_options::value<int>()->default_value(3),
         "length of test number (0..9)\n"
         "extra space will be filled by zeroes (003 instead of 3)\n"
         "if 0 - just number, without zeroes")
      ("start-from", boost::program_options::value<int>()->default_value(1),
         "test number (in file name) to start from");
}

void CTestWriter::run()
{
   emit started();
   boost::program_options::variables_map varMap;
   if(!readOptions(mArgs, varMap))
   {
      emit finished(1);
      return;
   }

   mFolder = QString::fromStdString(varMap["folder"].as<std::string>());
   mFolder = NFileSystem::get_full_path(mFolder);
   if(!NFileSystem::is_directory(mFolder))
   {
      emit error(" [ Error ] " + mFolder + " is not a directory\n");
      emit finished(1);
      return;
   }

   mNumLength = varMap["num-length"].as<int>();
   if(mNumLength < 0 || mNumLength > 9)
   {
      emit error(" [ Error ] Wrong number length\n");
      emit finished(1);
      return;
   }

   mStartFrom = varMap["start-from"].as<int>();
   if(mStartFrom < 0)
   {
      emit error(" [ Error ] Wrong number to start. Must be non-negative\n");
      emit finished(1);
      return;
   }

   auto patterns = parsePatternStr(varMap["pattern"].as<std::string>());
   if(patterns.first.empty() || patterns.second.empty())
   {
      emit error(" [ Error ] wrong pattern for file names\n");
      emit finished(1);
      return;
   }
   else
   {
      mInputPattern = QString::fromStdString(patterns.first);
      mOutputPattern = QString::fromStdString(patterns.second);
   }

   if(varMap.count("test"))
   {
      QString toWrite = QString::fromStdString(varMap["test"].as<std::string>());
      if(toWrite[0] == ':')
      {
         tList list = parseList(toWrite);
         if(validateList(list, 1000))
         {
            qDebug () << "list " << QVector<int>::fromStdVector(list);
            writeTests(list);
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
         tRange range = parseRange(toWrite);
         if(validateRange(range, 1000))
         {
            writeTests(range);
         }
         else
         {
            emit error(" [ Error ] wrong test range\n");
            emit finished(1);
            return;
         }
      }
   }
   else
   {
      tRange range(1, CTestProvider::getInstance().size());
      writeTests(range);
   }
}

void CTestWriter::terminate()
{
   mTerminatedFlag = true;
}

void CTestWriter::setArgs(const QStringList &args)
{
   mArgs = args;
}

void CTestWriter::writeTest(int idx)
{
   QString inputPath;
   QString outputPath;
   if(mNumLength == 0)
   {
      inputPath = mFolder + "/" + mInputPattern.arg(mStartFrom);
      outputPath = mFolder + "/" + mOutputPattern.arg(mStartFrom);
   }
   else
   {
      inputPath = mFolder + "/" + mInputPattern.arg(mStartFrom, mNumLength, 10, QChar('0'));
      outputPath = mFolder + "/" + mOutputPattern.arg(mStartFrom, mNumLength, 10, QChar('0'));
   }

   ++mStartFrom;

   const tTest& test = CTestProvider::getInstance().getTest(idx-1);
   QFile file;
   file.setFileName(inputPath);
   if(file.open(QFile::WriteOnly))
   {
       file.write(test.first.toLocal8Bit());
   }
   file.close();

   file.setFileName(outputPath);
   if(file.open(QFile::WriteOnly))
   {
       file.write(test.second.toLocal8Bit());
   }
}

void CTestWriter::writeTests(tRange range)
{
   for(int i = range.first; i <= range.second && !mTerminatedFlag; ++i)
   {
      writeTest(i);
      ++mTestsWritten;
   }
   emit finished(0);
}

void CTestWriter::writeTests(const tList &list)
{
   for(std::size_t i = 0; i < list.size() && !mTerminatedFlag; ++i)
   {
      writeTest(list[i]);
      ++mTestsWritten;
   }
   emit finished(0);
}

} // namespace NCommand
