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

QString colorize(const QString &text, QColor color)
{
   return "<font color=" + color.name() + ">" + text + "</font>";
}

QString convertTextToHtml(const QString &text)
{
   QString ret = text.toHtmlEscaped();
   ret.replace(QString("\n"), QString("<br>"));
   ret.replace(QString(" "), QString("&nbsp;"));
   return ret;
}

QString toHtmlSymbol(int code)
{
   return "&#" + QString::number(code) + ";";
}

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
