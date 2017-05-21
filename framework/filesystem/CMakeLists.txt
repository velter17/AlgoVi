set ( FILESYSTEM_LIB_NAME "algovi_filesystem" )

set ( FILESYSTEM_LIB_SOURCES
    "filesystem.hpp"
    "private/filesystem.cpp"
    "CWorkingDir.hpp"
    "private/CWorkingDir.cpp"
    )

add_library ( ${FILESYSTEM_LIB_NAME} STATIC ${FILESYSTEM_LIB_SOURCES} )

target_link_libraries ( ${FILESYSTEM_LIB_NAME}
    ${Boost_LIBRARIES}
    ${SETTINGS_LIB_NAME}
    )

qt5_use_modules ( ${FILESYSTEM_LIB_NAME} Core)

install (
    TARGETS
        ${FILESYSTEM_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
