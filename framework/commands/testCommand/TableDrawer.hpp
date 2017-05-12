/**
 * Project   AlgoVi
 *
 * @file     TableDrawer.hpp.hpp
 * @author   Dmytro Sadovyi
 * @date     09.05.2017
 */

#pragma once

#include <QString>
#include <vector>
#include <iostream>

namespace
{

#define LUBORDER 9484
#define RUBORDER 9488
#define LBBORDER 9492
#define RBBORDER 9496
#define HORBORDER 9472
#define VERBORDER 9474
#define BORDERBOTTOM 9516
#define BORDERRIGHT 9500
#define BORDERLEFT 9508
#define BORDERUP 9524
#define PLUSBORDER 9532

#define SPACE 160

} // anonymous namespace

namespace NCommand
{

enum class TAlign
{
    Left,
    Right,
    Center
};

class CCell
{
public: // methods
   CCell()
      : mAlign(TAlign::Left), mColor("#ffffff"), mCollspan(1)
   {}
#define ADD_ATTRIBUTE(name, type) \
   public: \
      const type& get##name() const \
      { return m##name; } \
      CCell& set##name(const type& item) \
      { \
         m##name = item; \
         return *this; \
      } \
   private: \
      type m##name;

ADD_ATTRIBUTE(Align, TAlign)
ADD_ATTRIBUTE(Color, QString)
ADD_ATTRIBUTE(Data, std::vector <QString>)
ADD_ATTRIBUTE(Collspan, int)
#undef ADD_ATTRIBUTE

public: // methods
    int height() const { return mData.size(); }
    int width() const { int ret = 0; for(const QString& str : mData) ret = std::max(ret, str.length()); return ret; }
};

inline QString toHtmlSymbol(int code)
{
   return "&#" + QString::number(code) + ";";
}

inline std::vector <std::vector <int>> getCellsWidth(const std::vector <std::vector <CCell>>& table)
{
   std::vector <std::vector <int> > ret(table.size());
   for(std::size_t i = 0; i < table.size(); ++i)
   {
      ret[i].resize(table[i].size(), 0);
   }
   std::vector <std::size_t> ptr(table.size(), 0);
   std::vector <int> cells(table.size());
   int completed = 0;
   for(int cell = 1; completed < table.size(); ++cell)
   {
      int mx = 0;
      for(std::size_t i = 0; i < table.size(); ++i)
      {
         if(ptr[i] == table[i].size())
         {
            continue;
         }
         if(cells[i] + table[i][ptr[i]].getCollspan() == cell)
         {
            int needAdd = std::max(0, table[i][ptr[i]].width() + 2 - ret[i][ptr[i]]);
            mx = std::max(mx, needAdd);
         }
      }
      for(std::size_t i = 0; i < table.size(); ++i)
      {
         if(ptr[i] == table[i].size())
         {
            continue;
         }
         ret[i][ptr[i]] += mx;
         if(cells[i] + table[i][ptr[i]].getCollspan() == cell)
         {
            cells[i] = cell;
            ++ptr[i];
            if(ptr[i] == ret[i].size())
            {
               ++completed;
            }
         }
      }
   }

   for(std::size_t i = 0; i < ret.size(); ++i)
   {
      for(std::size_t j = 0; j < ret[i].size(); ++j)
      {
         ret[i][j] += table[i][j].getCollspan() - 1;
      }
   }

   return ret;
}

inline QString getHtmlTable(const std::vector <std::vector <CCell>>& table, QString borderColor = "#ffffff")
{
    QString ret = "<font color=" + borderColor + ">";
    std::vector <int> h(table.size());
    for(std::size_t i = 0; i < table.size(); ++i)
    {
        for(std::size_t j = 0; j < table[i].size(); ++j)
        {
            h[i] = std::max(h[i], int(table[i][j].height()));
        }
    }

    std::vector <std::vector <int>> cellswidth = getCellsWidth(table);
    for(int i = 0; i < cellswidth.size(); ++i)
    {
       for(int j = 0; j < cellswidth[i].size(); ++j)
       {
          std::cout << cellswidth[i][j] << " ";
       }
       std::cout << std::endl;
    }

    auto drawTop = [&ret](const std::vector <int>& w)
    {
       ret += toHtmlSymbol(LUBORDER);
       for(std::size_t i = 0; i < w.size(); ++i)
       {
          ret += toHtmlSymbol(HORBORDER).repeated(w[i]);
          if(i == w.size()-1)
          {
             ret += toHtmlSymbol(RUBORDER);
          }
          else
          {
             ret += toHtmlSymbol(BORDERBOTTOM);
          }
       }
       ret += "<br>";
    };

    auto drawMid = [&ret](const std::vector <int>& w, const std::vector <int>& nxt)
    {
       std::vector <int> pt {0};
       int sumw = 0, sumnxt = 0;
       for(int t : w)
       {
          sumw += t+1;
          pt.push_back(sumw);
       }
       for(int t : nxt)
       {
          sumnxt += t+1;
          pt.push_back(sumnxt);
       }
       std::sort(pt.begin(), pt.end());
       int cnt = std::unique(pt.begin(), pt.end()) - pt.begin();
       pt.resize(cnt);
       std::cout << "pt: ";
       for(int t : pt)
       {
          std::cout << t << " ";
       }
       std::cout << std::endl;
       ret += toHtmlSymbol(BORDERRIGHT);
       std::size_t ptrw = 0, ptrnxt = 0;
       sumw = 0, sumnxt = 0;
       for(std::size_t i = 1; i < pt.size(); ++i)
       {
          ret += toHtmlSymbol(HORBORDER).repeated(pt[i] - pt[i - 1] - 1);
          if(i == pt.size()-1)
          {
             ret += toHtmlSymbol(BORDERLEFT);
          }
          else
          {
             if(sumw + w[ptrw] > pt[i])
             {
                ret += toHtmlSymbol(BORDERBOTTOM);
                sumnxt += nxt[ptrnxt++];
             }
             else if(sumnxt + nxt[ptrnxt] > pt[i])
             {
                ret += toHtmlSymbol(BORDERUP);
                sumw += w[ptrw++];
             }
             else
             {
                sumw += w[ptrw++];
                sumnxt += nxt[ptrnxt++];
                ret += toHtmlSymbol(PLUSBORDER);
             }
          }
       }
       ret += "<br>";
    };

    auto drawBottom = [&ret](const std::vector <int>& w)
    {
       ret += toHtmlSymbol(LBBORDER);
       for(std::size_t i = 0; i < w.size(); ++i)
       {
          ret += toHtmlSymbol(HORBORDER).repeated(w[i]);
          if(i == w.size()-1)
          {
             ret += toHtmlSymbol(RBBORDER);
          }
          else
          {
             ret += toHtmlSymbol(BORDERUP);
          }
       }
       ret += "<br>";
    };

    auto drawCellPart = [&ret](const CCell& cell, int needw, int idx)
    {
       QString toAppend = cell.getData().size() <= idx ? QString() : cell.getData()[idx];
       int spaces = needw - toAppend.length() - 2;
       toAppend = toAppend.replace(" ", toHtmlSymbol(SPACE));
       toAppend = toHtmlSymbol(SPACE) + toAppend + toHtmlSymbol(SPACE);
       if(!cell.getColor().isEmpty())
       {
          toAppend = "<font color=" + cell.getColor() + ">" + toAppend + "</font>";
       }
       if(cell.getAlign() == TAlign::Left)
       {
          ret += toAppend + toHtmlSymbol(SPACE).repeated(spaces);
       }
       else if(cell.getAlign() == TAlign::Right)
       {
          ret += toHtmlSymbol(SPACE).repeated(spaces) + toAppend;
       }
       else
       {
          ret += toHtmlSymbol(SPACE).repeated(spaces / 2) + toAppend +
                 toHtmlSymbol(SPACE).repeated(spaces - (spaces / 2));
       }
    };

    drawTop(cellswidth.front());
    for(std::size_t i = 0; i < table.size(); ++i)
    {
       for(int idx = 0; idx < h[i]; ++idx)
       {
          ret += toHtmlSymbol(VERBORDER);
          for(std::size_t j = 0; j < table[i].size(); ++j)
          {
             drawCellPart(table[i][j], cellswidth[i][j], idx);
             ret += toHtmlSymbol(VERBORDER);
          }
          ret += "<br>";
       }
       i == table.size()-1 ? drawBottom(cellswidth.back()) : drawMid(cellswidth[i], cellswidth[i+1]);
    }

    return ret + "</font>";
}

} // namespace NCommand

