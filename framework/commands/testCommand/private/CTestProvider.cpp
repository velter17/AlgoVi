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
    mData.push_back(test);
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

void CTestProvider::erase(int idx)
{
    if(idx >= mData.size())
    {
        qDebug () << "WARNING: erase wrong test";
        return;
    }
    mData.erase(mData.begin() + idx);
}

std::size_t CTestProvider::size()
{
   return mData.size();
}

CTestProvider::CTestProvider()
{

}


} // namespace NCommand
