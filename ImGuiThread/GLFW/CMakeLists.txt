cmake_minimum_required(VERSION 3.22)

project(GLFW)

find_package(OpenGL REQUIRED)

if(WIN32)
    add_library(GLFW SHARED IMPORTED GLOBAL)
    set_target_properties(GLFW PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/glfw3dll.lib"
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    target_link_libraries(GLFW INTERFACE OpenGL::GL)
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll" DESTINATION "${CMAKE_BINARY_DIR}")
elseif(UNIX)
    add_library(GLFW STATIC IMPORTED GLOBAL)
    set_target_properties(GLFW PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    find_package(X11 REQUIRED)
    find_package(Threads REQUIRED)
    target_link_libraries(GLFW INTERFACE
            OpenGL::GL
            X11::X11
            ${CMAKE_DL_LIBS}
            Threads::Threads
    )
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

#cmake_minimum_required(VERSION 3.10)
#project(GLFW)
#
#add_library(GLFW STATIC IMPORTED GLOBAL)
#set_target_properties(GLFW PROPERTIES
#        IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a
#)
#target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})