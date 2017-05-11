/**
 * Project   AlgoVi
 *
 * @file     TableDrawer.hpp.hpp
 * @author   Dmytro Sadovyi
 * @date     11.05.2017
 */

#pragma once

#include <QStringList>
#include <algorithm>
#include <string>

namespace NCommand
{

typedef std::pair<int, int> tRange;
typedef std::vector <int> tList;

inline bool validateNum(const QString& n)
{
   for(QChar c : n)
   {
      char ch = c.toLatin1();
      if(!isdigit(ch))
      {
         return false;
      }
   }
   return n.length() < 9;
}

inline bool validateRange(const tRange& range, int tests)
{
   return range.first > 0 && range.second <= tests;
}

inline bool validateList(const tList& list, int tests)
{
   for(int idx : list)
   {
      if(idx < 0 || idx > tests)
      {
         return false;
      }
   }
   return true;
}

inline tRange parseRange(const QString& str)
{
   QStringList list = str.split('-');
   bool nums = true;
   for(const QString& s : list)
   {
      nums &= validateNum(s);
   }
   if(!nums || list.isEmpty() || list.size() > 2)
   {
      return tRange(0, 0);
   }
   if(list.size() == 1)
   {
      int test =list.first().toInt();
      return tRange(test, test);
   }
   else
   {
      int from = (*list.begin()).toInt();
      int to = (*(list.begin() + 1)).toInt();
      return tRange(from, to);
   }
}

inline tList parseList(const QString& str)
{
   QStringList list = str.mid(1).split(",");
   bool nums = true;
   for(const QString& s : list)
   {
      nums &= validateNum(s);
   }
   if(!nums || list.isEmpty())
   {
      return tList();
   }
   tList ret;
   for(const QString& s : list)
   {
      ret.push_back(s.toInt());
   }
   return ret;
}

} // namespace NCommand
