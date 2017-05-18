set ( SETTINGS_LIB_NAME "algovi_settings" )

set ( SETTINGS_LIB_SOURCES
   "readers/CSettingsReader.hpp"
   "readers/private/CSettingsReader.cpp"
   "readers/CCommandConfigReader.hpp"
   "readers/private/CCommandConfigReader.cpp"
   "readers/CTerminalConfigReader.hpp"
   "readers/private/CTerminalConfigReader.cpp"
   "readers/CRunnerConfigReader.hpp"
   "readers/private/CRunnerConfigReader.cpp"
    )

add_library ( ${SETTINGS_LIB_NAME} STATIC ${SETTINGS_LIB_SOURCES} )

target_link_libraries ( ${SETTINGS_LIB_NAME}
    ${Boost_LIBRARIES}
    )

qt5_use_modules ( ${SETTINGS_LIB_NAME} Core Xml)

install (
    TARGETS
    ${SETTINGS_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )

install (
    DIRECTORY "configs"
    DESTINATION ${INSTALL_BIN_DIR}
    FILES_MATCHING PATTERN "*.xml"
)
