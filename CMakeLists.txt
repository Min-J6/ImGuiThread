cmake_minimum_required(VERSION 3.22)
project(main)

set(CMAKE_CXX_STANDARD 23)
#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake)
#set(CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/vcpkg/installed/x64-linux/share)

# 프로젝트 전체에 적용할 include 디렉토리
include_directories(${CMAKE_SOURCE_DIR})

# 서브 디렉토리에서 정적 라이브러리를 빌드
add_subdirectory(ImGuiThread)

# main.cpp 파일을 사용하여 실행 파일을 빌드
add_executable(${PROJECT_NAME} main.cpp)

# 실행 파일과 정적 라이브러리를 링크
target_link_libraries(${PROJECT_NAME} PRIVATE ImGuiThread)