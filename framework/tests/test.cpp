#include <bits/stdc++.h>
#include "Test.hpp"

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/XmlOutputter.h>

int main()
{
    CppUnit::TextUi::TestRunner runner;
    //CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
    runner.addTest( CTest::suite() );
    bool wasSuccessful = runner.run();
    std::ofstream resFile("unittestresult.xml");
    CppUnit::XmlOutputter xmlOut(&runner.result(), resFile);
    xmlOut.write();
    return !wasSuccessful;
}
