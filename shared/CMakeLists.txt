add_library(shared STATIC
    src/shared.cpp
)

target_include_directories(shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)