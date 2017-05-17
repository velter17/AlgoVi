/**
 * Project   AlgoVi
 *
 * @file     CTestReader.cpp
 * @author   Dmytro Sadovyi
 * @date     13.05.2017
 */

#include <QDebug>
#include <QFile>

#include "framework/commands/testCommand/CTestReader.hpp"
#include "framework/commands/testCommand/TestCommandHelper.hpp"
#include "framework/filesystem/filesystem.hpp"

namespace NCommand
{

CTestReader::CTestReader()
   : mTerminatedFlag(false)
   , mTestsRead(0)
{
   mOptions.add_options()
      ("folder,f", boost::program_options::value<std::string>()->required(),
         "source folder")
      ("test,t", boost::program_options::value<std::string>(),
         "tests for reading")
      ("pattern", boost::program_options::value<std::string>()->default_value("%1.(dat|ans)"),
         "file names pattern")
      ("num-length", boost::program_options::value<int>()->default_value(3),
         "length of test number (0..9)\n"
         "extra space will be filled by zeroes (003 instead of 3)\n"
         "if 0 - just number, without zeroes");
}

void CTestReader::run()
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
      QString toRead = QString::fromStdString(varMap["test"].as<std::string>());
      if(toRead[0] == ':')
      {
         tList list = parseList(toRead);
         if(validateList(list, 1000))
         {
            qDebug () << "list " << QVector<int>::fromStdVector(list);
            std::thread t1(&CTestReader::readTestsList, this, list);
            t1.detach();
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
         tRange range = parseRange(toRead);
         if(validateRange(range, 1000))
         {
            std::thread t1(&CTestReader::readTestsRange, this, range);
            t1.detach();
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
      tRange range(1, 1000);
      std::thread t1(&CTestReader::readTestsRange, this, range);
      t1.detach();
   }
}

void CTestReader::terminate()
{
   std::lock_guard<std::mutex> guard(mDataAccess);
   mTerminatedFlag = true;
}

void CTestReader::setArgs(const QStringList &args)
{
   mArgs = args;
}

tTest CTestReader::readTest(int idx)
{
   QString inputPath;
   QString outputPath;
   if(mNumLength == 0)
   {
      inputPath = mFolder + "/" + mInputPattern.arg(idx);
      outputPath = mFolder + "/" + mOutputPattern.arg(idx);
   }
   else
   {
      inputPath = mFolder + "/" + mInputPattern.arg(idx, mNumLength, 10, QChar('0'));
      outputPath = mFolder + "/" + mOutputPattern.arg(idx, mNumLength, 10, QChar('0'));
   }

   if(!NFileSystem::exists(inputPath) || !NFileSystem::exists(outputPath))
   {
       return tTest();
   }

   tTest test;
   QFile file;
   file.setFileName(inputPath);
   if(file.open(QFile::ReadOnly))
   {
       test.first = file.readAll();
   }
   file.close();

   file.setFileName(outputPath);
   if(file.open(QFile::ReadOnly))
   {
       test.second = file.readAll();
   }
   return test;
}

void CTestReader::readTestsRange(tRange range)
{
   for(int i = range.first; i <= range.second; ++i)
   {
      {
         std::lock_guard <std::mutex> guard(mDataAccess);
         if(mTerminatedFlag)
         {
            break;
         }
      }
      tTest test = readTest(i);
      if(!test.first.isEmpty() && !test.second.isEmpty())
      {
         ++mTestsRead;
         CTestProvider::getInstance().addTest(test);
      }
      else
      {
          break;
      }
   }
   emit log(" [ Info ] " + QString::number(mTestsRead) + " tests were read\n");
   emit finished(0);
}

void CTestReader::readTestsList(const tList &list)
{
   for(std::size_t i = 0; i < list.size(); ++i)
   {
      {
         std::lock_guard <std::mutex> guard(mDataAccess);
         if(mTerminatedFlag)
         {
            break;
         }
      }
      tTest test = readTest(list[i]);
      if(!test.first.isEmpty() && !test.second.isEmpty())
      {
         ++mTestsRead;
         CTestProvider::getInstance().addTest(test);
      }
      else
      {
          break;
      }
   }
   emit log(" [ Info ] " + QString::number(mTestsRead) + " tests were read\n");
   emit finished(0);
}

} // namespace NCommand
