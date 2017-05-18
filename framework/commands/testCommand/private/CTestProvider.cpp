/**
 * Project   AlgoVi
 *
 * @file     CTestProvider.cpp
 * @author   Dmytro Sadovyi
 * @date     09.05.2017
 */

#include <QDebug>

#include "../CTestProvider.hpp"

namespace NCommand
{

CTestProvider& CTestProvider::getInstance()
{
    static CTestProvider instance;
    return instance;
}

void CTestProvider::addTest(const tTest& test)
{
    tTest t = test;
    while(t.first.endsWith('\n'))
    {
       t.first.chop(1);
    }
    while(t.second.endsWith('\n'))
    {
       t.second.chop(1);
    }
    mData.push_back(t);
}

const tTest& CTestProvider::getTest(int idx)
{
    if(idx >= mData.size())
    {
        static tTest emptyTest;
        return emptyTest;
    }
    return mData[idx];
}

tShortTest CTestProvider::getShortTest(int idx)
{
   const static int sMaxLen = 100;
   const static int sMaxRows = 25;

   const tTest& test = getTest(idx);
   QStringList input = test.first.split('\n');
   if(input.size() > sMaxRows)
   {
      input = input.mid(0, sMaxRows);
      input.append("...");
      input.append("all size: " + QString::number(test.first.length()));
   }
   for(QString& str : input)
   {
      str = str.length() > sMaxLen ? str.mid(0, sMaxLen) + " ..." : str;
   }

   QStringList output = test.second.split('\n');
   if(output.size() > sMaxRows)
   {
      output = output.mid(0, sMaxRows);
      output.append("...");
      output.append("all size: " + QString::number(test.second.length()));
   }
   for(QString& str : output)
   {
      str = str.length() > sMaxLen ? str.mid(0, sMaxLen) + " ..." : str;
   }
   return tShortTest(input, output);
}

void CTestProvider::erase(int idx)
{
    if(idx >= mData.size())
    {
        qDebug () << "WARNING: erase wrong test: " << idx << ", but actual size is " << mData.size();
        return;
    }
    mData.erase(mData.begin() + idx);
}

void CTestProvider::erase(int from, int to)
{
   mData.erase(mData.begin() + from, mData.begin() + to + 1);
}

void CTestProvider::swap(int a, int b)
{
   if(a < 0 || a >= mData.size() || b < 0 || b >= mData.size())
   {
      qDebug () << "WARNING: swap wrong tests: " << a << " " << b;
      return;
   }
   std::swap(mData[a], mData[b]);
}

void CTestProvider::move(int from, int to)
{
   if(from < 0 || from >= mData.size() || to < 0 || to >= mData.size())
   {
      qDebug () << "WARNING: move wrong positions: " << from << " " << to;
      return;
   }
   tTest t = mData[from];
   mData.erase(mData.begin() + from);
   mData.insert(mData.begin() + to, t);
}

std::size_t CTestProvider::size()
{
   return mData.size();
}

void CTestProvider::clear()
{
   mData.clear();
}

bool CTestProvider::empty()
{
   return mData.empty();
}

CTestProvider::CTestProvider()
{
}


} // namespace NCommand
