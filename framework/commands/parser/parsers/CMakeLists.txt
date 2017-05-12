set ( CF_PARSER_NAME "algovi_codeforcesParser" )

set ( CF_PARSER_SOURCES
   "cf.cpp"
    )

add_executable ( ${CF_PARSER_NAME} ${CF_PARSER_SOURCES} )

install (
    TARGETS
    ${CF_PARSER_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}/parsers
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
