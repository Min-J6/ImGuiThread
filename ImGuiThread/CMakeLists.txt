cmake_minimum_required(VERSION 3.22)

project(ImGuiThread)

add_subdirectory(GL)
add_subdirectory(GLFW)

file(GLOB IMGUI_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/font/gg sans Medium.ttf" DESTINATION "${CMAKE_BINARY_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/image/gura.jpeg" DESTINATION "${CMAKE_BINARY_DIR}")

add_library(ImGuiThread STATIC ${IMGUI_SOURCES})

target_include_directories(ImGuiThread PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(ImGuiThread PUBLIC GL GLFW)

set_target_properties(ImGuiThread PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}"
)