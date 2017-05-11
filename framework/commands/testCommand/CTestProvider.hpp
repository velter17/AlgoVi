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

class CTestProvider
{
public: // methods
    static CTestProvider& getInstance();
    void addTest(const tTest& test);
    const tTest& getTest(int idx);
    void erase(int idx);
    std::size_t size();
private: // methods
    CTestProvider();
    CTestProvider(const CTestProvider&) = delete;
private: // fields
    std::vector <tTest> mData;
};

} // namespace NCommand

