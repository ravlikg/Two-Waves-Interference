cmake_minimum_required(VERSION 4.1)
project(interpolation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")

include(FetchContent)

FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.2
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(SFML)


FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG        v1.91.1
        GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(imgui)

FetchContent_GetProperties(imgui SOURCE_DIR IMGUI_SOURCE_DIR)
set(IMGUI_DIR ${IMGUI_SOURCE_DIR} CACHE PATH "Path to ImGui Top-Level Directory")


set(IMGUI_SFML_FIND_SFML OFF CACHE INTERNAL "Disable imgui-sfml internal find_package(SFML)")


FetchContent_Declare(imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG        v3.0
)
FetchContent_MakeAvailable(imgui-sfml)


add_executable(interpolation
        source/main.cpp
)

configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/wave.frag
        ${CMAKE_CURRENT_BINARY_DIR}/wave.frag
        COPYONLY
)

target_link_libraries(interpolation
        PRIVATE
        SFML::Graphics
        SFML::Window
        SFML::System
        ImGui-SFML::ImGui-SFML
)