cmake_minimum_required(VERSION 4.0)
project(Projekt C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fsanitize=address")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Projekt main.c
        src/world.c
        src/world.h
        src/object.h
        src/object.c
        src/basicStructs.h
        src/keybinds.h
        src/keybinds.c
        src/player.c
        src/player.h
        src/liveInput.c
        src/liveInput.h
        src/WorkingWithFiles.h
        src/WorkingWIthFiles.c
        src/collisions.c
        src/collisions.h
        src/animace.h
        src/animace.c
)
target_link_libraries(Projekt ${SDL2_LIBRARIES} SDL2_image)