set ( COMPLATION_LIB_NAME "algovi_complation" )

set ( COMPLATION_LIB_SOURCES
   "CComplationProvider.hpp"
   "private/CComplationProvider.cpp"
   "TCommandParserHelper.hpp"
    )

add_library ( ${COMPLATION_LIB_NAME} STATIC ${COMPLATION_LIB_SOURCES} )

target_link_libraries ( ${COMPLATION_LIB_NAME}
    ${Boost_LIBRARIES}
    )

qt5_use_modules ( ${COMPLATION_LIB_NAME} Core)

install (
    TARGETS
    ${COMPLATION_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
