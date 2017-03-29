set ( ALGOVI_GUI_LIB_NAME "algoviGui" )

set ( ALGOVI_GUI_LIB_SOURCES
    "CMainWindow.hpp"
    "private/CMainWindow.cpp"
    )

add_library ( ${ALGOVI_GUI_LIB_NAME} STATIC ${ALGOVI_GUI_LIB_SOURCES} )

target_link_libraries( ${ALGOVI_GUI_LIB_NAME}
    ${TERMINAL_LIB_NAME} )

qt5_use_modules ( ${ALGOVI_GUI_LIB_NAME} Widgets Core)

install (
    TARGETS
    ${ALGOVI_GUI_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
