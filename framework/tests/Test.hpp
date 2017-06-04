#include <bits/stdc++.h>
#include <cppunit/TestAssert.h>
#include <cppunit/extensions/HelperMacros.h>
#include "CClassToTest.hpp"

class CTest : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(CTest);
    CPPUNIT_TEST(funcA);
    CPPUNIT_TEST(funcB);
    CPPUNIT_TEST_SUITE_END();
public:
    void funcA();
    void funcB();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CTest);

void CTest::funcA()
{
    CClassToTest obj(3, 5);
    CPPUNIT_ASSERT_MESSAGE("Wrong min function", obj.min() == 3);
    CPPUNIT_ASSERT_MESSAGE("Wrong max function", obj.max() == 5);
}

void CTest::funcB()
{
    CClassToTest obj(4, 1);
    CPPUNIT_ASSERT_MESSAGE("Wrong xor function", obj.XOR() == 5);
    CPPUNIT_ASSERT_MESSAGE("Wrong plus function", obj.plus() == 5);
    CPPUNIT_ASSERT_MESSAGE("Wrong minus function", obj.minus() == 3);
}
