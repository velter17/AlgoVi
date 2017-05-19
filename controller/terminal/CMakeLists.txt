set ( TERMINAL_LIB_NAME "terminal" )

set ( TERMINAL_LIB_SOURCES
    "ITerminal.hpp"
    "CTerminalBase.hpp"
    "private/CTerminalBase.cpp"
    "CPlainTextTerminal.hpp"
    "private/CPlainTextTerminal.cpp"
    "private/CPlainTextTerminalImpl.hpp"
    "private/CPlainTextTerminalImpl.cpp"
    "TerminalHelpers.hpp"
    "private/TerminalHelpers.cpp"
    ""
    )

add_library ( ${TERMINAL_LIB_NAME} STATIC ${TERMINAL_LIB_SOURCES} )

target_link_libraries ( ${TERMINAL_LIB_NAME}
    ${FILESYSTEM_LIB_NAME}
    ${COMPLATION_LIB_NAME}
    ${SETTINGS_LIB_NAME}
    )

qt5_use_modules ( ${TERMINAL_LIB_NAME} Widgets Core)

install (
    TARGETS
    ${TERMINAL_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
