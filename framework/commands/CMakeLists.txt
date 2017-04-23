set ( TERMINAL_COMMANDS_LIB_NAME "terminalCommands" )

set ( TERMINAL_COMMANDS_LIB_SOURCES
    "ITerminalCommand.hpp"
    "ITerminalSignals.hpp"
    "CTerminalCommandBase.hpp"
    "private/CTerminalCommandBase.cpp"
    "CSystemCommand.hpp"
    "private/CSystemCommand.cpp"
    "CPythonShell.hpp"
    "private/CPythonShell.cpp"
    "CAppExecutor.hpp"
    "private/CAppExecutor.cpp"
    )

add_library ( ${TERMINAL_COMMANDS_LIB_NAME} STATIC ${TERMINAL_COMMANDS_LIB_SOURCES} )

target_link_libraries ( ${TERMINAL_COMMANDS_LIB_NAME}
    ${Boost_LIBRARIES}
    )

qt5_use_modules ( ${TERMINAL_COMMANDS_LIB_NAME} Core)

install (
    TARGETS
        ${TERMINAL_COMMANDS_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
