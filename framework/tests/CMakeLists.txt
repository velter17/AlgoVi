set ( TESTS_LIB_NAME "some_tests" )

set ( TESTS_LIB_SOURCES
   "CClassToTest.hpp"
   "CClassToTest.cpp"
    )

add_library ( ${TESTS_LIB_NAME} STATIC ${TESTS_LIB_SOURCES} )


qt5_use_modules ( ${TESTS_LIB_NAME} Core Xml)

install (
    TARGETS
    ${TESTS_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )

add_executable ( test_unit_firstTest "test.cpp" )
target_link_libraries ( test_unit_firstTest
    ${TESTS_LIB_NAME}
    ${CPPUNIT_LIBRARIES}
    )

install (
    TARGETS
    test_unit_firstTest
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}/tests/
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )


