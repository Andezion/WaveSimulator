cmake_minimum_required(VERSION 3.16)
project(WaveSimulator CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find raylib installed on the system first
find_package(raylib 4.0 QUIET)
if (NOT raylib_FOUND)
    include(FetchContent)
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG        5.0
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(raylib)
endif()

file(GLOB_RECURSE SOURCES
    "main.cpp"
    "app_state.cpp"
    "waves_math/*.cpp"
    "window_handlers/*.cpp"
    "button_handlers/*.cpp"
    "input_handlers/*.cpp"
    "file_handlers/*.cpp"
    "camera_handlers/*.cpp"
)

add_executable(WaveSimulator ${SOURCES})

target_include_directories(WaveSimulator PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(WaveSimulator PRIVATE raylib)

if(UNIX AND NOT APPLE)
    target_link_libraries(WaveSimulator PRIVATE m dl pthread)
    # X11 / OpenGL needed on Linux
    find_package(OpenGL REQUIRED)
    target_link_libraries(WaveSimulator PRIVATE OpenGL::GL)
    find_package(X11)
    if(X11_FOUND)
        target_link_libraries(WaveSimulator PRIVATE X11::X11)
    endif()
endif()

# Export compile commands for clangd / IDE linter
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
