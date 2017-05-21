set ( ALGOVI_GUI_LIB_NAME "algoviGui" )

set ( ALGOVI_GUI_LIB_SOURCES
    "CMainWindow.hpp"
    "private/CMainWindow.cpp"
    "CTestEditor.hpp"
    "private/CTestEditor.cpp"
    )

add_library ( ${ALGOVI_GUI_LIB_NAME} STATIC ${ALGOVI_GUI_LIB_SOURCES} )

target_link_libraries( ${ALGOVI_GUI_LIB_NAME}
    ${TERMINAL_LIB_NAME}
    )

if( ALGOVI_HAS_QTWEBKIT )
    target_link_libraries( ${ALGOVI_GUI_LIB_NAME}
	    ${NOVILE_EDITOR_LIB_NAME}
	    )
endif()

qt5_use_modules ( ${ALGOVI_GUI_LIB_NAME} Widgets Core)

install (
    TARGETS
    ${ALGOVI_GUI_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
