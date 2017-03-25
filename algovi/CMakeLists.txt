set ( ALGOVI_APP_NAME "algovi" )

file ( GLOB_RECURSE ALGOVI_APP_SOURCES
    "*.hpp"
    "*.cpp"
)

add_executable ( ${ALGOVI_APP_NAME} ${ALGOVI_APP_SOURCES} )

install (
    TARGETS 
        ${ALGOVI_APP_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
)
