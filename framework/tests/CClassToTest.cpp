
#include "CClassToTest.hpp"

CClassToTest::CClassToTest(int a, int b)
    : mA(a)
    , mB(b)
{
}

int CClassToTest::max()
{
    return std::max(mA, mB);
}

int CClassToTest::min()
{
    return std::min(mA, mB);
}

int CClassToTest::XOR()
{
    return mA ^ mB;
}

int CClassToTest::plus()
{
    return mA + mB;
}

int CClassToTest::minus()
{
    return mA - mB;
}
