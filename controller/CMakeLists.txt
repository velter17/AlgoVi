set ( ALGOVI_CONTROLLER_LIB_NAME "algoviController" )

set ( ALGOVI_CONTROLLER_LIB_SOURCES
    "CController.hpp"
    "private/CController.cpp"
)

add_library ( ${ALGOVI_CONTROLLER_LIB_NAME} STATIC ${ALGOVI_CONTROLLER_LIB_SOURCES} )

target_link_libraries ( ${ALGOVI_CONTROLLER_LIB_NAME}
   ${ALGOVI_GUI_LIB_NAME}
   ${SETTINGS_LIB_NAME}
   ${ALGOVI_SCENE_LIB_NAME} # to delete
   ${ALGOVI_VIEW_LIB_NAME} # to delete
)

qt5_use_modules ( ${ALGOVI_CONTROLLER_LIB_NAME} Widgets Core)

install (
    TARGETS
    ${ALGOVI_CONTROLLER_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )


add_subdirectory ( terminal )
