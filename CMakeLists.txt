cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(project_ironfall)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(remote)
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(external)