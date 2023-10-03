cmake_minimum_required(VERSION 3.26)
project(snekon)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    webgpu
    GIT_REPOSITORY https://github.com/eliemichel/WebGPU-distribution
    GIT_TAG        wgpu
)
FetchContent_MakeAvailable(webgpu)

FetchContent_Declare(
        glfw
        GIT_REPOSITORY https://github.com/glfw/glfw
        GIT_TAG        master
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
        glfw3webgpu
        GIT_REPOSITORY https://github.com/eliemichel/glfw3webgpu
        GIT_TAG        main
)
FetchContent_MakeAvailable(glfw3webgpu)

add_executable(snekon
        src/main.cpp
        src/game/game.cpp
        src/game/game.h
        src/game/snake.cpp
        src/game/snake.h
        src/game/window/renderer.cpp
        src/game/window/renderer.h
        src/game/window/window.cpp
        src/game/window/window.h
        src/game/window/input.h
        src/game/utility/timer.cpp
        src/game/utility/timer.h
        src/game/utility/utility.h
)

target_link_libraries(snekon PRIVATE webgpu glfw glfw3webgpu)

target_copy_webgpu_binaries(snekon)
