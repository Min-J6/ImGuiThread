cmake_minimum_required(VERSION 3.28)
project(test_window_glfw)

set(CMAKE_CXX_STANDARD 17)


if (WIN32)
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif ()

add_subdirectory(ImGuiThread)

add_executable(test_window_glfw main.cpp)

target_link_libraries(test_window_glfw PRIVATE ImGuiThread)
