set ( ALGOVI_SCENE_LIB_NAME "algovi_scene" )

set ( ALGOVI_SCENE_LIB_SOURCES
    "CGraphicView.hpp"
    "private/CGraphicView.cpp"
    "CGraphicsSceneBase.hpp"
    "private/CGraphicsSceneBase.cpp"
    )

add_library ( ${ALGOVI_SCENE_LIB_NAME} STATIC ${ALGOVI_SCENE_LIB_SOURCES} )

#target_link_libraries( ${ALGOVI_SCENE_LIB_NAME}
#    )

qt5_use_modules ( ${ALGOVI_SCENE_LIB_NAME} Widgets Core)

install (
    TARGETS
    ${ALGOVI_SCENE_LIB_NAME}
    RUNTIME DESTINATION ${INSTALL_BIN_DIR}
    LIBRARY DESTINATION ${INSTALL_LIB_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIB_DIR}
    )
