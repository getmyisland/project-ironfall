cmake_minimum_required(VERSION 3.15 FATAL_ERROR)
project(dyxide)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external)
add_subdirectory(engine)
add_subdirectory(game)