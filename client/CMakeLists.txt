find_package(OpenGL REQUIRED)

add_executable(client
    src/main.cpp
    ${CMAKE_SOURCE_DIR}/external/glad/glad.c
)

target_include_directories(client PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/remote/include
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glm
)

target_link_libraries(client PRIVATE
    remote
    ${OPENGL_LIBRARIES}
    glfw
	glm
)

# Copy resources
add_custom_command(TARGET client POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:client>/resources")