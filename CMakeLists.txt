cmake_minimum_required(VERSION 4.0)
project(Projekt C)

set(CMAKE_C_STANDARD 11)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Projekt main.c)
target_link_libraries(Projekt ${SDL2_LIBRARIES} SDL2_image)