cmake_minimum_required(VERSION 3.25)
project(QProj LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)

include("${CMAKE_CURRENT_LIST_DIR}/cmake/module.cmake")

add_subdirectory("module/qstruct")
add_subdirectory("module/json")
add_subdirectory("module/qui_core")