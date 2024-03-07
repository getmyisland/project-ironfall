find_package(OpenGL REQUIRED)

add_library(engine STATIC
    src/Application.cpp
    ${CMAKE_SOURCE_DIR}/external/glad/glad.c
)

target_include_directories(engine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    ${CMAKE_SOURCE_DIR}/shared/include
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glm
)

target_link_libraries(engine PRIVATE
    shared
    ${OPENGL_LIBRARIES}
    glfw
	glm
)

# Copy resources
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:engine>/resources")