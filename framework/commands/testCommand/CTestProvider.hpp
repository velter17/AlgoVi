/**
 * Project   AlgoVi
 *
 * @file     CTestProvider.hpp
 * @author   Dmytro Sadovyi
 * @date     09.05.2017
 */

#pragma once

#include <QPair>
#include <QString>

namespace NCommand
{

typedef QPair<QString, QString> tTest;
typedef QPair<QStringList, QStringList> tShortTest;

class CTestProvider
{
public: // methods
    static CTestProvider& getInstance();
    void addTest(const tTest& test);
    void setTest(int idx, const tTest& test);
    const tTest& getTest(int idx);
    tShortTest getShortTest(int idx);
    void erase(int idx);
    void erase(int from, int to);
    void swap(int a, int b);
    void move(int from, int to);
    std::size_t size();
    void clear();
    bool empty();
private: // methods
    CTestProvider();
    CTestProvider(const CTestProvider&) = delete;
private: // fields
    std::vector <tTest> mData;
};

} // namespace NCommand

