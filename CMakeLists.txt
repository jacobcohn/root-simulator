cmake_minimum_required(VERSION 3.22)
project(root_simulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)

if(EMSCRIPTEN)
    set(PLATFORM Web CACHE STRING "" FORCE)
endif()

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.tar.gz
    URL_HASH SHA256=aea98ecf5bc5c5e0b789a76de0083a21a70457050ea4cc2aec7566935f5e258e
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(raylib)

add_executable(root_simulator
    src/main.cpp
    src/app.cpp
    src/viewport.cpp
    src/orbit_camera.cpp
    src/root_generator.cpp
    src/root_presets.cpp
    src/root_scene.cpp
    src/root_controls.cpp
    src/overlay.cpp
)
target_include_directories(root_simulator PRIVATE include)
target_link_libraries(root_simulator PRIVATE raylib)

if(EMSCRIPTEN)
    set_target_properties(root_simulator PROPERTIES
        OUTPUT_NAME "root_simulator"
        SUFFIX ".js"
    )

    target_compile_definitions(root_simulator PRIVATE PLATFORM_WEB)
    target_link_options(root_simulator PRIVATE
        "-sUSE_GLFW=3"
        "-sWASM=1"
        "-sALLOW_MEMORY_GROWTH=1"
        "-sASYNCIFY"
        "-sNO_EXIT_RUNTIME=1"
        "-sENVIRONMENT=web"
    )

    add_custom_command(TARGET root_simulator POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/public/index.html
            $<TARGET_FILE_DIR:root_simulator>/index.html
    )
endif()
