cmake_minimum_required(VERSION 3.10)

project(GLFW)

if(WIN32)
    add_library(GLFW SHARED IMPORTED GLOBAL)
    set_target_properties(GLFW PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/glfw3dll.lib"
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../"
    )
    target_link_libraries(GLFW INTERFACE opengl32 glu32)

    # DLL 파일 복사
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.dll" DESTINATION "${CMAKE_BINARY_DIR}")
elseif(UNIX)
    add_library(GLFW STATIC IMPORTED GLOBAL)
    set_target_properties(GLFW PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libglfw3.a"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
    )
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(GLFW INTERFACE OpenGL::GL X11::X11 dl pthread)
else()
    message(FATAL_ERROR "Unsupported platform")
endif()