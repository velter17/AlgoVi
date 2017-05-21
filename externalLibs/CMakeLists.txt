INCLUDE_DIRECTORIES ( testlib )
ADD_SUBDIRECTORY ( testlib )

if (ALGOVI_HAS_QTWEBKIT)
    INCLUDE_DIRECTORIES ( novileEditor )
    ADD_SUBDIRECTORY ( novileEditor )
endif ()
