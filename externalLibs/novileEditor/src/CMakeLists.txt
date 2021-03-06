#
# This file is part of the Novile Editor
#
# This program is free software licensed under the GNU LGPL. You can
# find a copy of this license in LICENSE in the top directory of
# the source code.
#
# Copyright 2013    Illya Kovalevskyy   <illya.kovalevskyy@gmail.com>
#

set(NOVILE_EDITOR_LIB_NAME "novile" )

set(NOVILE_SOURCES
    editor.cpp
)

set(NOVILE_PUBLIC_HEADER
    editor.h
    novile_export.h
)

set(NOVILE_PUBLIC_INCLUDE
    ../include/NovileEditor
)

qt5_add_resources(NOVILE_RCC_SRC ../data/shared.qrc)
add_library(${NOVILE_EDITOR_LIB_NAME} SHARED ${NOVILE_SOURCES}
                          ${NOVILE_RCC_SRC}
                          ../data/shared.qrc
)
qt5_use_modules(novile WebKitWidgets)

set_target_properties(novile PROPERTIES
    DEFINE_SYMBOL NOVILE_MAKEDLL
    PUBLIC_HEADER "${NOVILE_PUBLIC_HEADER}"
)

install(TARGETS novile
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
)

#install(FILES ${NOVILE_PUBLIC_INCLUDE}
#        DESTINATION ${CMAKE_INSTALL_PREFIX}/include
#)
