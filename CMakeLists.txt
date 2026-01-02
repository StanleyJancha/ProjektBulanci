cmake_minimum_required(VERSION 4.0)
project(Projekt C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -g -fsanitize=address -LSAN_OPTIONS=verbosity=1:log_threads=1")
#set(CMAKE_BUILD_TYPE Debug)
#set(CMAKE_C_FLAGS_DEBUG "-g -O0")

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
        src/SavingManager.h
        src/SavingManager.c
        src/collisions.c
        src/collisions.h
        src/animace.h
        src/animace.c
        src/render.c
        src/render.h
        src/weapons.c
        src/weapons.h
        src/basicStructs.c
        src/ui.c
        src/ui.h
        src/gamerule.h
        src/gamerule.c
        src/ai.c
        src/ai.h
)
target_link_libraries(Projekt ${SDL2_LIBRARIES} SDL2_image)
target_link_libraries(Projekt ${SDL2_LIBRARIES} SDL2_ttf)


set(ASSET_DIRS "animations" "animations_ui" "fonts")

foreach(DIR ${ASSET_DIRS})
    add_custom_command(TARGET Projekt POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/${DIR}"
            "${CMAKE_CURRENT_BINARY_DIR}/${DIR}"
            COMMENT "Syncing ${DIR} to build directory..."
    )
endforeach()