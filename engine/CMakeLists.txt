find_package(OpenGL REQUIRED)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# Glob recursively for all source files
file(GLOB_RECURSE SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
)

# Add external dependencies
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/external/glad/src/glad.c")
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/external/entt/include/entt.hpp")
list(APPEND SOURCES "${CMAKE_SOURCE_DIR}/external/stb_image/stb_image.cpp")

add_library(engine STATIC ${SOURCES})

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/external/glad/include
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/entt/include
	${CMAKE_SOURCE_DIR}/external/msdf-atlas-gen/msdf-atlas-gen
	${CMAKE_SOURCE_DIR}/external/msdf-atlas-gen/msdfgen
	${CMAKE_SOURCE_DIR}/external/msdf-atlas-gen/msdfgen/include
	${CMAKE_SOURCE_DIR}/external/msdf-atlas-gen/msdfgen/freetype/include
    ${CMAKE_SOURCE_DIR}/external/glm
    ${CMAKE_SOURCE_DIR}/external/stb_image
)

target_link_libraries(engine PUBLIC
    ${OPENGL_LIBRARIES}
    glfw
	glm
	msdf-atlas-gen
	msdfgen
	freetype
)