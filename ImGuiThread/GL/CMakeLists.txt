cmake_minimum_required(VERSION 3.22)

project(GL)

find_package(OpenGL REQUIRED)

add_library(GL INTERFACE)
target_include_directories(GL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    add_library(GLEW SHARED IMPORTED)
    set_target_properties(GLEW PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/glew32.lib"
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/glew32.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    target_link_libraries(GL INTERFACE GLEW OpenGL::GL)
else()
    add_library(GLEW STATIC IMPORTED)
    set_target_properties(GLEW PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libGLEW.a"
    )
    target_link_libraries(GL INTERFACE GLEW OpenGL::GL)
endif()

if(UNIX AND NOT APPLE)
    find_package(X11 REQUIRED)
    target_link_libraries(GL INTERFACE ${X11_LIBRARIES})
endif()

target_compile_definitions(GL INTERFACE GLEW_STATIC)

#cmake_minimum_required(VERSION 3.10)
#project(GL)
#
#add_library(GL STATIC IMPORTED GLOBAL)
#set_target_properties(GL PROPERTIES
#        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libGLEW.a
#)
#target_include_directories(GL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})