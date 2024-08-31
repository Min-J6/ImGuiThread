#include <iostream>

#include "ImGuiThread.h"

int main()
{
    int num = 50;
    while (true) {
        ImGuiThread::begin("test window", [&]() {
            ImGui::Text("Hello, world!");
            ImGui::SliderInt("Number", &num, 0, 100);
            ImGui::Text("Number: %d", num);
        });

        std::this_thread::sleep_for(std::chrono::milliseconds(160)); // 약 60 FPS
    }
}

// ImGuiThread::invoke("demo_window", []() {
//             static bool show_demo_window = true;
//             ImGui::ShowDemoWindow(&show_demo_window);
//         });
//
// ImGuiThread::invoke("plot_demo_window", []() {
//     static bool show_plot_demo_window = true;
//     ImPlot::ShowDemoWindow(&show_plot_demo_window);
// });
//
//
// ImGuiThread::begin("test window", [&]() {
//     ImGui::Text("Hello, world!");
//     ImGui::SliderInt("Number", &num, 0, 100);
//     ImGui::Text("Number: %d", num);
// });