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
find_package ( Qt5WebKit )
if( Qt5WebKit_FOUND )
    message (STATUS "ALGOVI HAS Qt5WebKit")
    set ( ALGOVI_HAS_QTWEBKIT ON)
    add_definitions(-DALGOVI_HAS_QTWEBKIT)
endif()
find_package ( Boost REQUIRED COMPONENTS 
    filesystem
    system
    program_options
    )
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
find_package ( CppUnit REQUIRED)

set ( BOOST_USE_STATIC_LIBS ON )
set ( BOOST_USE_MULTITHREADED ON )

#include dirs
include_directories( ${PROJECT_SOURCE_DIR} )
include_directories ( ${Boost_INCLUDE_DIRS} )

set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread" )

if ( "${CMAKE_BUILD_TYPE}" STREQUAL "Release" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DQT_NO_DEBUG_OUTPUT" )
endif()

include ( "build/Libraries.cmake" )

IF ( "${ALGOVI_TARGET_SYSTEM}" STREQUAL "" )
   IF ( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
      SET (ALGOVI_TARGET_SYSTEM "win32")
   ELSEIF ( ${CMAKE_SYSTEM_NAME} STREQUAL "Linux" )
      SET ( ALGOVI_TARGET_SYSTEM "linux" )
   ELSE ()
      MESSAGE ( SEND_ERROR "Unknown target system to build: '${CMAKE_SYSTEM_NAME}'" )
   ENDIF()
ENDIF ()

add_subdirectory ( "algovi" )
add_subdirectory ( "framework" )
add_subdirectory ( "gui" )
add_subdirectory ( "controller" )
add_subdirectory ( "externalLibs" )
