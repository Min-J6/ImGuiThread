cmake_minimum_required(VERSION 3.10)
project(GLFW)

add_library(GLFW STATIC IMPORTED GLOBAL)
set_target_properties(GLFW PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a
)
target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})