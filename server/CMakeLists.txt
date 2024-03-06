add_executable(server
    src/main.cpp
)

target_link_libraries(server
    PRIVATE
        remote
)

target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/remote/include
)