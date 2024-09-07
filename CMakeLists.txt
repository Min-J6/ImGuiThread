cmake_minimum_required(VERSION 3.22)
project(main)

set(CMAKE_CXX_STANDARD 17)


if (WIN32)
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
endif ()

add_subdirectory(ImGuiThread) # ImGuiThread 라이브러리 추가

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE ImGuiThread) # ImGuiThread 라이브러리 링크
