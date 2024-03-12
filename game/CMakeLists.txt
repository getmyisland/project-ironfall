find_package(OpenGL REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Glob recursively for all source files
file(GLOB_RECURSE SRC_FILES "${SRC_DIR}/*.cpp" "${SRC_DIR}/*.c")

# Add the library
add_executable(game ${SRC_FILES})

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/engine/src
)

target_link_libraries(game PRIVATE
    engine
)

# Copy resources
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game>/resources")