cmake_minimum_required(VERSION 3.10)

project(ImGuiThread)

add_subdirectory(GL)
add_subdirectory(GLFW)

# ImGui 소스 파일 추가
file(GLOB IMGUI_SOURCES "*.cpp" "*.h")

# ImGuiThread 라이브러리 생성
add_library(ImGuiThread STATIC ${IMGUI_SOURCES})

# ImGui 헤더 파일 경로 추가
target_include_directories(ImGuiThread PUBLIC ${IMGUI_SOURCES})

target_link_libraries(ImGuiThread PUBLIC GL GLFW)
