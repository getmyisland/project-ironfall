add_executable(server
    src/Main.cpp
)

target_link_libraries(server
    PRIVATE
        shared
)

target_include_directories(server PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/shared/include
)