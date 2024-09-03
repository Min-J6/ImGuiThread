cmake_minimum_required(VERSION 3.10)

project(GL)

# GL 헤더 파일들을 포함하는 인터페이스 라이브러리 생성
add_library(GL INTERFACE)

# GL 폴더를 include 경로에 추가
target_include_directories(GL INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}../)

# GLEW 라이브러리 설정
if(WIN32)
    add_library(GLEW SHARED IMPORTED)
    set_target_properties(GLEW PROPERTIES
            IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/glew32.lib"
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/glew32.dll"
            INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/../"
    )
    if(NOT EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/glew32.lib")
        message(FATAL_ERROR "glew32.lib not found in ${CMAKE_CURRENT_SOURCE_DIR}")
    endif()
    target_include_directories(GLEW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(GL INTERFACE GLEW)

    # GLEW DLL 파일 복사
#    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/glew32.dll" DESTINATION "${CMAKE_BINARY_DIR}")
else()
    add_library(GLEW STATIC IMPORTED)
    set_target_properties(GLEW PROPERTIES
            IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/libglew.a"
    )
    target_include_directories(GLEW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
    target_link_libraries(GL INTERFACE GLEW)
endif()

# 플랫폼별 OpenGL 설정
if(WIN32)
    target_link_libraries(GL INTERFACE opengl32 glu32)
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(GL INTERFACE OpenGL::GL)
    # Linux에서 필요한 추가 라이브러리
    target_link_libraries(GL INTERFACE X11 Xrandr Xinerama Xi Xxf86vm Xcursor)
endif()

# 모든 타겟에 GL 자동 링크
#function(link_gl_to_target target)
#    if(TARGET ${target})
#        get_target_property(target_type ${target} TYPE)
#        if(target_type STREQUAL "EXECUTABLE" OR target_type STREQUAL "SHARED_LIBRARY" OR target_type STREQUAL "STATIC_LIBRARY")
#            target_link_libraries(${target} PRIVATE GL)
#        endif()
#    endif()
#endfunction()
#
## 새로운 타겟이 생성될 때마다 GL 링크
#cmake_language(DEFER DIRECTORY ${CMAKE_SOURCE_DIR} CALL link_gl_to_target ${CMAKE_PROJECT_NAME})