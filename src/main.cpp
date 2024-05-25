#include "nfd.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <glad/gl.h>
// GLAD BEFORE GLFW
#include <GLFW/glfw3.h>
#include <fstream>
#include <nfd.hpp>
#include <spdlog/spdlog.h>
#include <string>
#include <window.hpp>

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui.h"

#include "ezgl.hpp"

using namespace glm;

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
}

#define SHORTCUT_OPEN (GLFW_KEY_O | GLFW_MOD_CONTROL) // Ctrl+O
#define SHORTCUT_SAVE (GLFW_KEY_S | GLFW_MOD_CONTROL) // Ctrl+S
#define SHORTCUT_EXIT (GLFW_KEY_F4 | GLFW_MOD_ALT)    // Alt+F4

bool IsShortcutPressed(int shortcut)
{
    ImGuiIO &io = ImGui::GetIO();
    int key = shortcut & 0xFF;
    int mods = shortcut & ~0xFF;
    bool isPressed = io.KeysDown[key];

    if (mods & GLFW_MOD_CONTROL)
        isPressed &= io.KeyCtrl;
    if (mods & GLFW_MOD_SHIFT)
        isPressed &= io.KeyShift;
    if (mods & GLFW_MOD_ALT)
        isPressed &= io.KeyAlt;
    if (mods & GLFW_MOD_SUPER)
        isPressed &= io.KeySuper;

    return isPressed;
}

void fileDialog(std::string &filepath, std::string &filecontent)
{

    char *outPath;
    nfdfilteritem_t filterItem[2] = {
        {"Source code", "pml"},
    };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY)
    {
        filepath = outPath;
        spdlog::info("Trying to open file {}", filepath);
        std::ifstream file(filepath);
        if (!file)
        {
            spdlog::error("Couldn't open file {}", filepath);
            return;
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        filecontent = buffer.str();
    }
}

int main()
{
    // spdlog::set_level(spdlog::level::debug);
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    // Create a Window
    Window window(1280, 720, "Ray Tracer");

    if (NFD_Init() != NFD_OKAY)
    {
        spdlog::error("Could not load NFD, you cannot open files");
    }

    // Set callback

    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/Roboto.ttf", 100);
    io.Fonts->AddFontFromFileTTF("fonts/FiraCode.ttf", 100);
    io.Fonts->AddFontFromFileTTF("fonts/SauceCodePro.ttf", 100);
    io.Fonts->Build();
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts | ImGuiConfigFlags_DpiEnableScaleViewports;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(20, 20);
    style.FramePadding = ImVec2(20, 0);
    style.ItemSpacing = ImVec2(20, 6);
    style.ItemInnerSpacing = ImVec2(20, 0);
    style.GrabMinSize = 20;
    style.TabRounding = 2;

    // Scale Configuration
    float global_scale = 0.35;
    io.FontGlobalScale = global_scale;

    // Variables for custom styles
    const char *styles[] = {"Dark", "Light"};
    int current_style = 0;

    // Filepath variables
    std::string filepath = "";
    std::string filecontent = "";
    bool window_open = true;

    while (!window.shouldClose())
    {
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING

        window.startDrawing();

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dockspace", &window_open, window_flags);
        io.FontGlobalScale = global_scale * ImGui::GetWindowDpiScale();
        /* ImGuiID dockspace_id = ImGui::GetID("MyDockSpace"); */
        /* ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f)); */

        if (IsShortcutPressed(SHORTCUT_OPEN))
        {
            fileDialog(filepath, filecontent);
        }

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Open", "Ctrl+O"))
                {
                    fileDialog(filepath, filecontent);
                }
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        if (ImGui::BeginTabBar("Tabs"))
        {
            if (ImGui::BeginTabItem("Edit/View"))
            {
                ImGui::Text("Edit/View Content");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Simulate/Replay"))
            {
                ImGui::Text("Simulate/Replay Content");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Verification"))
            {
                ImGui::Text("Verification Content");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Swarm Run"))
            {
                ImGui::Text("Swarm Run Content");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("<Help>"))
            {
                ImGui::Text("Help Content");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Settings"))
            {
                ImGui::ShowFontSelector("Font");

                ImGui::DragFloat("Scale", &global_scale, 0.005f, 0.1, 2.0, "%.2f", ImGuiSliderFlags_AlwaysClamp);

                if (ImGui::Combo("Styles", &current_style, styles, IM_ARRAYSIZE(styles)))
                {
                    switch (current_style)
                    {
                    case 0:
                        ImGui::StyleColorsDark();
                        break;
                    case 1:
                        ImGui::StyleColorsLight();
                        break;
                    }
                }

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
        ImGui::ShowStyleEditor();

        window.endDrawing();

        // END RENDERING
    }

    NFD_Quit();
    return EXIT_SUCCESS;
}
