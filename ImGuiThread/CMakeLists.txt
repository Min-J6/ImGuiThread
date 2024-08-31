file(GLOB IMGUI_SOURCES "*.cpp" "*.h")

# ImGui 라이브러리 생성 (정적 라이브러리)
add_library(ImGuiThread STATIC ${IMGUI_SOURCES})

# ImGui 헤더 파일 경로 설정
target_include_directories(ImGuiThread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# glfw3 패키지를 찾아서 링크
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ImGuiThread PUBLIC glfw)

# GLEW 패키지를 찾아서 링크
find_package(GLEW REQUIRED)
target_link_libraries(ImGuiThread PRIVATE GLEW::GLEW)