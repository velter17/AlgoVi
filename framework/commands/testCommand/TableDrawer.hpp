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
#undef ADD_ATTRIBUTE

public: // methods
    int height() const { return mData.size(); }
    int width() const { int ret = 0; for(const QString& str : mData) ret = std::max(ret, str.length()); return ret; }
};

inline QString toHtmlSymbol(int code)
{
   return "&#" + QString::number(code) + ";";
}

inline QString getHtmlTable(const std::vector <std::vector <CCell>>& table, QString borderColor = "#ffffff")
{
    QString ret = "<font color=" + borderColor + ">";
    std::vector <int> w(table.back().size());
    std::vector <int> h(table.size());
    for(std::size_t i = 0; i < table.size(); ++i)
    {
        for(std::size_t j = 0; j < table[i].size(); ++j)
        {
            w[j] = std::max(w[j], int(table[i][j].width()) + 2);
            h[i] = std::max(h[i], int(table[i][j].height()));
        }
    }
    int width = 1 + w.size();
    for(std::size_t i = 0; i < w.size(); ++i)
    {
        width += w[i];
    }

    auto drawTop = [&w, &ret]()
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

    auto drawMid = [&w, &ret]()
    {
       ret += toHtmlSymbol(BORDERRIGHT);
       for(std::size_t i = 0; i < w.size(); ++i)
       {
          ret += toHtmlSymbol(HORBORDER).repeated(w[i]);
          if(i == w.size()-1)
          {
             ret += toHtmlSymbol(BORDERLEFT);
          }
          else
          {
             ret += toHtmlSymbol(PLUSBORDER);
          }
       }
       ret += "<br>";
    };

    auto drawBottom = [&w, &ret]()
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

    drawTop();
    for(std::size_t i = 0; i < table.size(); ++i)
    {
       for(int idx = 0; idx < h[i]; ++idx)
       {
          ret += toHtmlSymbol(VERBORDER);
          for(std::size_t j = 0; j < table[i].size(); ++j)
          {
             drawCellPart(table[i][j], w[j], idx);
             ret += toHtmlSymbol(VERBORDER);
          }
          ret += "<br>";
       }
       i == table.size()-1 ? drawBottom() : drawMid();
    }

    return ret + "</font>";
}

} // namespace NCommand

