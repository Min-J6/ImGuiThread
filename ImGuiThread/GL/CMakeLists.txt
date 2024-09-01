cmake_minimum_required(VERSION 3.10)
project(GL)

add_library(GL STATIC IMPORTED GLOBAL)
set_target_properties(GL PROPERTIES
        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libGLEW.a
)
target_include_directories(GL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})