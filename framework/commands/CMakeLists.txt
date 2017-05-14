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
    "CCompiler.hpp"
    "private/CCompiler.cpp"
    "private/CCompilerExecutor.hpp"
    "private/CCompilerExecutor.cpp"

    "testCommand/CTestProvider.hpp"
    "testCommand/private/CTestProvider.cpp"
    "testCommand/CTestCommand.hpp"
    "testCommand/private/CTestCommand.cpp"
    "testCommand/TableDrawer.hpp"
    "testCommand/TestCommandHelper.hpp"
    "testCommand/CTestReader.hpp"
    "testCommand/private/CTestReader.cpp"
    "testCommand/CTestWriter.hpp"
    "testCommand/private/CTestWriter.cpp"

    "parser/CParserCommand.hpp"
    "parser/private/CParserCommand.cpp"

    "tester/CTesterCommand.hpp"
    "tester/private/CTesterCommand.cpp"
    "tester/private/CTesterImpl.hpp"
    "tester/private/CTesterImpl.cpp"
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
