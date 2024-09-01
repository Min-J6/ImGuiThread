file(GLOB IMGUI_SOURCES "*.cpp" "*.h")

add_subdirectory(GLFW)
add_subdirectory(GL)

# OpenGL 패키지 찾기
find_package(OpenGL REQUIRED)

# X11 패키지 찾기 (Linux 환경을 위해)
find_package(X11 REQUIRED)

# ImGui 라이브러리 생성 (정적 라이브러리)
add_library(ImGuiThread STATIC ${IMGUI_SOURCES})

# ImGui 헤더 파일 경로 설정
target_include_directories(ImGuiThread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ImGuiThread
        PRIVATE
        GL
        GLFW
        ${OPENGL_LIBRARIES}
        ${X11_LIBRARIES}
        dl
        pthread
)