/**
 * Project   AlgoVi
 *
 * @file     TerminalHelpers.cpp
 * @author   Dmytro Sadovyi
 * @date     06.04.2017
 */

#include "../TerminalHelpers.hpp"

namespace NController
{

int calcLCP(const QStringList& list)
{
   if(list.empty())
   {
      return 0;
   }
   int lcp = 0;
   while(true)
   {
      const QString& s = list.first();
      if(s.length() == lcp)
      {
         break;
      }
      QChar need = s[lcp];
      bool flag = true;
      for(const QString& str : list)
      {
         if(str.length() <= lcp || str[lcp] != need)
         {
            flag = false;
            break;
         }
      }
      if(!flag)
      {
         break;
      }
      ++lcp;
   }
   return lcp;
}

} // namespace NController
