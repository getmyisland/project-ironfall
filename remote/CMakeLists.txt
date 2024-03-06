add_library(remote STATIC
    src/remote.cpp
)

target_include_directories(remote PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(remote PROPERTIES
    WINDOWS_EXPORT_ALL_SYMBOLS ON
    IMPORT_PREFIX ""
)