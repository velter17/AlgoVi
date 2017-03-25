cmake_minimum_required ( VERSION 3.0 )

project ( AlgoVi )

set ( INSTALL_BIN_DIR "bin" )
set ( INSTALL_LIB_DIR "lib" )

set ( CMAKE_AUTOMOC ON )
set ( CMAKE_AUTOUIC ON )

# dependencies
find_package ( Qt5Widgets REQUIRED )                                                                                                                                                                                                       
find_package ( Qt5Quick REQUIRED )                                                                                                                                                               
find_package ( Qt5Core REQUIRED )                                                                                                                                                                                                          
find_package ( Qt5Network REQUIRED )                                                                                                                                                                                                       
find_package ( Qt5Xml REQUIRED )                                                                                                                                                                                                           
find_package ( Boost REQUIRED COMPONENTS 
    filesystem
    system
    program_options
    )

set ( BOOST_USE_STATIC_LIBS ON )
set ( BOOST_USE_MULTITHREADED ON )

#include dirs
include_directories ( ${Boost_INCLUDE_DIRS} )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" )

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_DEBUD_OUTPUT" )
endif()

add_subdirectory ( "algovi" )
#add_subdirectory ( "framework" )

