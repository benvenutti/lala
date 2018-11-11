add_library( libpd SHARED IMPORTED GLOBAL )

set( libPath ${CMAKE_SOURCE_DIR}/3rd-party/libpd )

set( libpdIncludes
    ${libPath}/cpp
    ${libPath}/libpd_wrapper 
    ${libPath}/libpd_wrapper/util 
    ${libPath}/pure-data/src )

set_target_properties( libpd
    PROPERTIES
    IMPORTED_LOCATION ${libPath}/bin/libpd.so
    INTERFACE_INCLUDE_DIRECTORIES "${libpdIncludes}" )
