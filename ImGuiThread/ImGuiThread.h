#pragma once
#include <condition_variable>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <atomic>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include "ImageLoader.h"

#define IMGUI_THREAD_IMAGE_PATH "gura.jpeg"
#define IMGUI_THREAD_FONT_PATH  "gg sans Medium.ttf"
#define IMGUI_THREAD_FONT_SIZE  21
#define IMGUI_THREAD_SHOW_LOGO

class ImGuiThread {
public:
    static void invoke(const std::string& id, std::function<void()> command) {
        getInstance().addCommand(id, command);
    }

    template<typename Func>
    static void begin(const std::string& id, Func&& func) {
        static std::unordered_map<std::string, bool> windowStates;
        getInstance().addCommand(id, [id, func = std::forward<Func>(func)]() {
            if (ImGui::Begin(id.c_str(), &windowStates[id])) {
                func();
            }
            ImGui::End();
        });
    }

    static void toggleVisibility() {
        getInstance().isVisible = !getInstance().isVisible;
    }

    static bool isRunning() {
        return getInstance().running;
    }



private:
    struct GLFWwindowDeleter {
        void operator()(GLFWwindow* window) {
            glfwDestroyWindow(window);
        }
    };

    bool isVisible = false;
    bool isImageButtonVisible = true;

    ImGuiThread() : running(false), frameCount(0) {}
    ~ImGuiThread() { stop(); }

    ImGuiThread(const ImGuiThread&) = delete;
    ImGuiThread& operator=(const ImGuiThread&) = delete;

    static ImGuiThread& getInstance() {
        static ImGuiThread instance;
        return instance;
    }

    void addCommand(const std::string& id, std::function<void()> command) {
        static bool firstCommand = true;
        if (firstCommand) {
            start();
            firstCommand = false;
        }
        std::lock_guard<std::mutex> lock(queueMutex);
        commandQueue[id] = std::move(command);
        queueCV.notify_one();
    }

    void removeCommand(const std::string& id) {
        std::lock_guard<std::mutex> lock(queueMutex);
        commandQueue.erase(id);
        queueCV.notify_one();
    }

    void run() {
        try {
            if (!initializeGLFW() || !initializeImGui()) {
                throw std::runtime_error("Failed to initialize GLFW or ImGui");
            }

            running = true;
            while (running && !glfwWindowShouldClose(window.get())) {
                glfwPollEvents();

                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                if (isVisible) {
                    executeCommands();
                }

                renderImageButton();

                if (!isImageButtonVisible) {
                    running = false;  // 이미지 버튼 창이 닫히면 루프 종료
                }

                ImGui::Render();
                int display_w, display_h;
                glfwGetFramebufferSize(window.get(), &display_w, &display_h);
                glViewport(0, 0, display_w, display_h);
                glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
                glClear(GL_COLOR_BUFFER_BIT);
                ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                ImGuiIO& io = ImGui::GetIO();
                if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
                    GLFWwindow* backup_current_context = glfwGetCurrentContext();
                    ImGui::UpdatePlatformWindows();
                    ImGui::RenderPlatformWindowsDefault();
                    glfwMakeContextCurrent(backup_current_context);
                }

                glfwSwapBuffers(window.get());
                frameCount++;
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error in ImGuiThread: " << e.what() << std::endl;
        }

        cleanup();
    }

    void renderImageButton() {
        static ImageLoader imageLoader;
        static ImVec2 imageSize;

        // 이미지 크기 계산 (최초 1회)
        static bool first = true;
        if (first) {
            first = false;
            imageLoader.loadImage(IMGUI_THREAD_IMAGE_PATH);
            imageSize = ImVec2(imageLoader.getWidth(), imageLoader.getHeight());

            // 모니터 해상도 가져오기
            GLFWmonitor* primary = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(primary);

            // 윈도우 위치 계산 (우측 하단)
            ImVec2 monitorSize(mode->width, mode->height);
            ImVec2 windowPos(monitorSize.x - (imageSize.x + 5), (monitorSize.y - 230));

            // 윈도우 위치와 크기를 항상 설정
            ImGui::SetNextWindowPos(windowPos, ImGuiCond_Always);
            ImGui::SetNextWindowSize(imageSize, ImGuiCond_Always);
        }

        // 창 스타일 설정
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        // 버튼 스타일 설정
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

        if (ImGui::Begin("ImGuiThread", &isImageButtonVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
            imageSize = ImGui::GetContentRegionAvail();
            if (ImGui::ImageButton((void*)(intptr_t)imageLoader.getTexture(), imageSize)) {
                toggleVisibility();
            }
        }
        ImGui::End();

        // 스타일 복원
        ImGui::PopStyleVar(4);
        ImGui::PopStyleColor(3);
    }

    bool initializeGLFW() {
        if (!glfwInit()) {
            return false;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* rawWindow = glfwCreateWindow(1, 1, "Stable ImGui Thread Example", NULL, NULL);
        if (rawWindow == NULL) {
            glfwTerminate();
            return false;
        }

        window = std::unique_ptr<GLFWwindow, GLFWwindowDeleter>(rawWindow);

        glfwMakeContextCurrent(window.get());
        glfwSwapInterval(1); // Enable v-sync

        if (glewInit() != GLEW_OK) {
            return false;
        }

        glfwHideWindow(window.get());  // 창을 숨김

        return true;
    }

    bool initializeImGui() {
        IMGUI_CHECKVERSION();
        imguiContext = std::unique_ptr<ImGuiContext, ImGuiContextDeleter>(ImGui::CreateContext());
        implotContext = std::unique_ptr<ImPlotContext, ImPlotContextDeleter>(ImPlot::CreateContext());

        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        ImGui::StyleColorsDark();

        // 기본 폰트 변경
        io.Fonts->Clear(); // 기존 폰트를 제거
        io.Fonts->AddFontFromFileTTF(IMGUI_THREAD_FONT_PATH, 23.0f);
        io.Fonts->Build();

        ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
        ImGui_ImplOpenGL3_Init("#version 330");

        return true;
    }


    void executeCommands() {
        std::unordered_map<std::string, std::function<void()>> currentCommands;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait_for(lock, std::chrono::milliseconds(16), [this] { return !commandQueue.empty() || !running; });
            currentCommands = commandQueue;
        }

        if (glfwWindowShouldClose(window.get())) {
            return;
        }

        for (const auto& [id, command] : currentCommands) {
            command();
        }
    }

    void cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        glfwTerminate();
    }

    void start() {
        thread = std::make_unique<std::thread>(&ImGuiThread::run, this);
    }

    void stop() {
        running = false;
        queueCV.notify_one();
        if (thread && thread->joinable()) {
            thread->join();
        }
    }

    struct ImGuiContextDeleter {
        void operator()(ImGuiContext* ctx) { ImGui::DestroyContext(ctx); }
    };

    struct ImPlotContextDeleter {
        void operator()(ImPlotContext* ctx) { ImPlot::DestroyContext(ctx); }
    };

    std::unique_ptr<GLFWwindow, GLFWwindowDeleter> window;
    std::unique_ptr<ImGuiContext, ImGuiContextDeleter> imguiContext;
    std::unique_ptr<ImPlotContext, ImPlotContextDeleter> implotContext;
    std::unique_ptr<std::thread> thread;
    std::atomic<bool> running;
    std::unordered_map<std::string, std::function<void()>> commandQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::atomic<uint64_t> frameCount;
};
