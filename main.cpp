
#include <iostream>
#include "ImGuiThread.h"

// 전역 변수
std::atomic<int> global_counter{ 0 };
std::atomic<bool> keep_running{ true };

// 다른 스레드에서 실행될 함수
void worker_thread() {
    while (keep_running) {
        // 전역 변수 증가
        global_counter++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

int main() {
    // 작업 스레드 시작
    std::thread worker(worker_thread);

    while (true) {
        // Window 1에서 전역 변수 출력
        ImGuiThread::begin("Window 1", []() {
            ImGui::Text("Counter: %d", global_counter.load());
        });

        // Window 2에서 전역 변수 수정
        ImGuiThread::begin("Window 2", []() {
            static int local_counter = 0;
            local_counter++;
            if (ImGui::Button("Increment Global Counter")) {
                global_counter++;
            }
            ImGui::Text("Local Counter: %d", local_counter);
        });

        // 특정 명령 실행
        ImGuiThread::invoke("Command 1", []() {
            if (ImGui::Button("Stop Worker Thread")) {
                keep_running = false;
            }
        });

        // CPU 사용량을 줄이기 위해 작은 지연 추가
        std::this_thread::sleep_for(std::chrono::milliseconds(16));

        // 작업 스레드를 멈추도록 명령이 실행된 경우 루프 종료
        if (!keep_running) break;
    }

    // 작업 스레드가 종료되기를 기다림
    worker.join();

    return 0;
}
