set ( ALGOVI_APP_NAME "algovi" )

file ( GLOB_RECURSE ALGOVI_APP_SOURCES
    "*.hpp"
    "*.cpp"
)

add_executable ( ${ALGOVI_APP_NAME} ${ALGOVI_APP_SOURCES} )

target_link_libraries( ${ALGOVI_APP_NAME}
    ${ALGOVI_GUI_LIB_NAME}
    ${ALGOVI_CONTROLLER_LIB_NAME}
    ${TERMINAL_COMMANDS_LIB_NAME}
    )

qt5_use_modules ( ${ALGOVI_APP_NAME} Core Widgets Network)

install (
    TARGETS 
        ${ALGOVI_APP_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)
