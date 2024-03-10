find_package(OpenGL REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Glob recursively for all source files
file(GLOB_RECURSE SRC_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Add external dependencies
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/external/entt/include/entt.hpp")
list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.cpp")

add_library(engine STATIC ${SRC_FILES})

target_include_directories(engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:src>
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/entt/include
	${CMAKE_SOURCE_DIR}/external/freetype/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/stb_image
)

target_link_libraries(engine PUBLIC
    ${OPENGL_LIBRARIES}
    glfw
	glm
	freetype
)

# Copy resources
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:engine>/resources")