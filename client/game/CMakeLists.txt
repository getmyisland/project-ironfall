find_package(OpenGL REQUIRED)

add_executable(game
    src/Game.cpp
)

target_include_directories(game PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/engine/include
    ${CMAKE_SOURCE_DIR}/shared/include
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/glfw/include
    ${CMAKE_SOURCE_DIR}/external/glm
)

target_link_libraries(game PRIVATE
    shared
    engine
    ${OPENGL_LIBRARIES}
    glfw
	glm
)

# Copy resources
add_custom_command(TARGET game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/resources"
        "$<TARGET_FILE_DIR:game>/resources")