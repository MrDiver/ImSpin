#include "imgui_internal.h"
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

void RenderEditView(std::string filepath, std::string filecontent)
{
    ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockSpaceId = ImGui::GetID("MyDockSpace");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
    ImGui::Begin("Dock1");
    ImGui::End();
    ImGui::Begin("Dock2");
    ImGui::End();
    ImGui::Begin("Dock3");
    ImGui::End();
}

ImVec4 RGB(int r, int g, int b, int a)
{
    return ImVec4(r / 255.0, g / 255.0, b / 255.0, a / 255.0);
}

void CatppuccinMocha()
{
    auto &style = ImGui::GetStyle();
    ImVec4 *colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(0.79f, 0.83f, 0.96f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.43f, 0.45f, 0.55f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.12f, 0.13f, 0.19f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border] = ImVec4(0.41f, 0.22f, 0.44f, 0.27f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.71f, 0.75f, 1.00f, 0.99f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.12f, 0.12f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.65f, 0.97f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.85f, 0.42f, 0.54f, 1.00f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.85f, 0.42f, 0.54f, 1.00f);
    colors[ImGuiCol_Button] = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.35f, 0.36f, 0.44f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_Separator] = ImVec4(0.41f, 0.22f, 0.44f, 1.00f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.94f, 0.37f, 0.53f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.80f, 0.65f, 0.97f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.85f, 0.42f, 0.54f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.20f, 0.20f, 0.35f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.41f, 0.21f, 0.44f, 1.00f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.20f, 0.20f, 0.35f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.41f, 0.22f, 0.44f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.94f, 0.36f, 0.53f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.89f, 0.77f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.85f, 0.24f, 0.34f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(0.98f, 0.70f, 0.53f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.80f, 0.51f, 0.33f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.12f, 0.12f, 0.19f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.16f, 0.16f, 0.24f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.56f, 0.27f, 0.48f, 0.35f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.75f, 1.00f, 0.57f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(0.94f, 0.36f, 0.53f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}

void BasicStyling()
{
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->AddFontFromFileTTF("fonts/Roboto.ttf", 100);
    io.Fonts->AddFontFromFileTTF("fonts/FiraCode.ttf", 100);
    io.Fonts->AddFontFromFileTTF("fonts/SauceCodePro.ttf", 100);
    io.Fonts->Build();

    ImGuiStyle &style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(20, 20);
    style.FramePadding = ImVec2(20, 6);
    style.CellPadding = ImVec2(20, 6);
    style.ItemSpacing = ImVec2(20, 6);
    style.ItemInnerSpacing = ImVec2(20, 0);
    style.GrabMinSize = 20;
    style.TabRounding = 4;
    style.FrameRounding = 4;
    style.ScrollbarRounding = 12;
    style.GrabRounding = 3;
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.SeparatorTextBorderSize = 2;
    style.SeparatorTextAlign = ImVec2(0.5, 0.5);
    style.SeparatorTextPadding = ImVec2(20, 18);
    style.FrameBorderSize = 1;
    style.WindowBorderSize = 1;
    style.PopupBorderSize = 1;
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

    BasicStyling();
    CatppuccinMocha();

    ImGuiIO &io = ImGui::GetIO();
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

        io.FontGlobalScale = global_scale * ImGui::GetWindowDpiScale();

        // Start config for Main window
        const ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                                        ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                                        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        // ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("TabWindow", &window_open, window_flags);
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
                RenderEditView(filepath, filecontent);
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
        ImGui::PopStyleVar(2);
        ImGui::ShowStyleEditor();
        ImGui::ShowDemoWindow();

        window.endDrawing();

        // END RENDERING
    }

    NFD_Quit();
    return EXIT_SUCCESS;
}
