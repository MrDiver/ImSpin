#include "imgui_internal.h"
#include "nfd.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <boost/process.hpp>
#include <boost/xpressive/xpressive.hpp>
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

#include "TextEditor.h"
#include "config.hpp"
#include "ezgl.hpp"
#include "fonts.hpp"
#include "spinwrap.hpp"

using namespace glm;
using namespace boost::process;
namespace fs = std::filesystem;

// FIXME: Make flag to not overwrite layout when imgui.config exists

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
}

#define SHORTCUT_OPEN (ImGuiKey_O | ImGuiMod_Ctrl)                  // Ctrl+O
#define SHORTCUT_SAVE (ImGuiKey_S | ImGuiMod_Ctrl)                  // Ctrl+S
#define SHORTCUT_EXIT (ImGuiKey_F4 | ImGuiMod_Alt)                  // Alt+F4
#define SHORTCUT_FORMAT (ImGuiKey_F | ImGuiMod_Alt | ImGuiMod_Ctrl) // Ctrl+Alt+F

void fileDialogPml(std::string &filepath)
{
    char *outPath;
    nfdfilteritem_t filterItem[2] = {
        {"Source code", "pml"},
    };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY)
    {
        filepath = outPath;
    }
}

void fileDialogExe(std::string &filepath)
{
    char *outPath;
    nfdfilteritem_t filterItem[2] = {
        {"Executable", "exe"},
    };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY)
    {
        filepath = outPath;
    }
}

void fileDialogTrail(std::string &filepath)
{
    char *outPath;
    nfdfilteritem_t filterItem[2] = {
        {"Trail", "trail"},
    };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, 1, NULL);
    if (result == NFD_OKAY)
    {
        filepath = outPath;
    }
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

void RebuildFonts(float size)
{
    ImGuiIO &io = ImGui::GetIO();
    io.Fonts->ClearFonts();
    io.Fonts->Clear();
    ImFontConfig cfg;
    cfg.OversampleH = 4;
    cfg.OversampleV = 4;
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(Roboto_compressed_data_base85, size, &cfg);
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(Fira_compressed_data_base85, size);
    io.Fonts->AddFontFromMemoryCompressedBase85TTF(SauceCodePro_compressed_data_base85, size);
    // io.Fonts->AddFontFromFileTTF("fonts/SauceCodePro.ttf", size);
    io.Fonts->Build();
}

void BasicStyling()
{

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

void RenderMenuBar(std::string &filepath, bool &fileOpened, bool &fileSaved, bool &fileFormat)
{
    fileOpened = false;
    fileSaved = false;
    fileFormat = false;
    if (ImGui::IsKeyChordPressed(SHORTCUT_OPEN))
    {
        fileDialogPml(filepath);
        fileOpened = true;
    }
    /* if (ImGui::IsKeyChordPressed(SHORTCUT_FORMAT))
     {
         fileFormat = true;
     }*/

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                fileDialogPml(filepath);
                fileOpened = true;
            }

            // if (ImGui::MenuItem("Format", "Ctrl+Alt+F"))
            //{
            //     fileFormat = true;
            // }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void SetupDockEditView()
{
    static bool editViewDockInitialized = false;
    if (!editViewDockInitialized)
    {
        static ImGuiID dockSpaceId = ImGui::GetID("EditViewDock");
        ImGuiID dockUp, dockDown, dockLeft, dockRight;

        ImGui::DockBuilderRemoveNode(dockSpaceId);
        ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetWindowSize());
        dockDown = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down, 0.25, nullptr, &dockSpaceId);
        dockRight = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.25, nullptr, &dockSpaceId);
        ImGui::DockBuilderDockWindow("Editor", dockSpaceId);
        ImGui::DockBuilderDockWindow("Automata View", dockRight);
        ImGui::DockBuilderDockWindow("Log", dockDown);
        ImGui::DockBuilderFinish(dockSpaceId);
        editViewDockInitialized = true;
    }
}

void RenderEditView(TextEditor &editor)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    SetupDockEditView();

    ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockSpaceId = ImGui::GetID("EditViewDock");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);

    ImGui::Begin("Editor");
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    editor.Render("promelathing");
    ImGui::PopFont();
    ImGui::End();

    // ImGui::SetNextWindowDockID(dockRight);
    ImGui::Begin("Automata View");
    ImGui::End();

    // ImGui::SetNextWindowDockID(dockDown);
    ImGui::Begin("Log");
    ImGui::End();

    ImGui::PopStyleVar(1);
}

void SetupDockSimulateView()
{
    static bool editViewDockInitialized = false;
    if (!editViewDockInitialized)
    {
        static ImGuiID dockSpaceId = ImGui::GetID("SimulateViewDock");
        ImGuiID dockUp, dockDown, dockLeft, dockRight;

        ImGui::DockBuilderRemoveNode(dockSpaceId);
        ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetWindowSize());
        dockDown = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down, 0.25, nullptr, &dockSpaceId);
        dockRight = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.25, nullptr, &dockSpaceId);
        ImGui::DockBuilderDockWindow("Editor", dockSpaceId);
        ImGui::DockBuilderDockWindow("Automata View", dockRight);
        ImGui::DockBuilderDockWindow("Log", dockDown);
        ImGui::DockBuilderFinish(dockSpaceId);
        editViewDockInitialized = true;
    }
}

void RenderSimulationView(TextEditor &editor)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // SetupDockEditView();

    ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockSpaceId = ImGui::GetID("SimulateViewDock");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);
    // ImGui::SetNextWindowDockID(dockRight);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(20, 20));
    if (ImGui::Begin("Simulation Settings", NULL))
    {
        if (ImGui::BeginTable("SplitTable", 3,
                              ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings |
                                  ImGuiTableFlags_BordersInner))
        {
            ImGui::TableNextColumn();
            {
                ImGui::Text("Mode");
                ImGui::RadioButton("Random with seed", (int *)&config.simulationMode, RANDOM);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputInt("##Seed", &config.seed);
                ImGui::PopItemWidth();
                ImGui::RadioButton("Interactive (for resolution of all nondeterminism)", (int *)&config.simulationMode,
                                   INTERACTIVE);
                ImGui::RadioButton("Guided, with trail: ", (int *)&config.simulationMode, GUIDED);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Browse").x - 60);
                ImGui::InputText("##TrailPath", config.trailPath, 256);
                ImGui::PopItemWidth();
                ImGui::SameLine();
                if (ImGui::Button("Browse"))
                {
                    std::string tmp;
                    fileDialogTrail(tmp);
                    strncpy(config.trailPath, tmp.c_str(), tmp.size());
                }
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputInt("Initial steps skipped", &config.initialStepsSkipped);
                ImGui::InputInt("Maximum number of steps", &config.maximumNumberOfSteps);
                ImGui::PopItemWidth();
                ImGui::Checkbox("Track Data Values (this can be slow)", &config.trackDataValues);
            }
            ImGui::TableNextColumn();
            {
                ImGui::Text("A Full Channel");
                ImGui::RadioButton("blocks new messages", (int *)&config.channelMode, BLOCKING);
                ImGui::RadioButton("loses new messages", (int *)&config.channelMode, LOSING);
                ImGui::Checkbox("MSC+stmnt", &config.MSCStmnt);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("MSC max text width").x -
                                     20);
                ImGui::InputInt("MSC max text width", &config.MSCMaxTextWidth, 1, 1000);
                ImGui::InputInt("MSC update delay", &config.MSCUpdateDelay, 1, 1000);
                ImGui::PopItemWidth();
            }
            ImGui::TableNextColumn();
            {
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Tracked Variable").x - 20);
                ImGui::Text("Output Filtering [regex]");
                ImGui::InputText("Process Ids", config.filterProcessIds, 256);
                ImGui::InputText("Queue Ids", config.filterQueueIds, 256);
                ImGui::InputText("Var Names", config.filterVarNames, 256);
                ImGui::InputText("Tracked Variable", config.filterTrackedVariable, 256);
                ImGui::InputText("Track Scaling", config.filterTrackScaling, 256);
                ImGui::PopItemWidth();
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
    if (ImGui::Begin("Controls"))
    {
        int tmp = ImGui::CalcTextSize(" Step Backward   ").x;
        ImGui::Button("(Re)Run", ImVec2(tmp, 0));
        ImGui::Button("Stop", ImVec2(tmp, 0));
        ImGui::Button("Rewind", ImVec2(tmp, 0));
        ImGui::Button("Step Forward", ImVec2(tmp, 0));
        ImGui::Button("Step Backward", ImVec2(tmp, 0));
    }
    ImGui::End();
    if (ImGui::Begin("Command"))
    {
        int tmp = ImGui::CalcTextSize(" Step Backward   ").x;
        ImGui::Text("spin ");
    }
    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Begin("Simulation Editor");
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    editor.Render("promelathing");
    ImGui::PopFont();
    ImGui::End();

    // ImGui::SetNextWindowDockID(dockDown);
    ImGui::Begin("Simulation Log");
    ImGui::End();

    ImGui::PopStyleVar(1);
}

std::vector<std::string> getProgramFilesPaths()
{
    std::vector<std::string> paths;
    char *programFiles = getenv("ProgramFiles");
    char *programFilesX86 = getenv("ProgramFiles(x86)");

    if (programFiles)
    {
        paths.push_back(std::string(programFiles));
    }
    if (programFilesX86)
    {
        paths.push_back(std::string(programFilesX86));
    }

    return paths;
}

std::string locateVSExecutable(std::string filename)
{
    namespace fs = std::filesystem;
    std::vector<std::string> programFilesPaths = getProgramFilesPaths();
    std::string relativePath = "\\Microsoft Visual Studio\\";

    for (const auto &basePath : programFilesPaths)
    {
        std::string searchPath = basePath + relativePath;

        try
        {
            for (const auto &dirEntry : fs::recursive_directory_iterator(searchPath))
            {
                if (dirEntry.path().filename() == filename)
                {
                    return dirEntry.path().string();
                }
            }
        }
        catch (const std::exception &)
        {
            // Skip directories that can't be accessed
        }
    }

    return filename + " not found!";
}

int main()
{
    using namespace boost::process;
    using namespace boost::xpressive;

    // Reading Config
    {
        try
        {
            std::ifstream file("config.json");
            if (file)
            {
                std::stringstream buffer;
                buffer << file.rdbuf();
                std::string config_content = buffer.str();
                nlohmann::json j = nlohmann::json::parse(config_content);
                config = j.template get<Config>();
            }
        }
        catch (std::exception e)
        {
            spdlog::warn("Failed to load config.json");
        }
    }
    config.tmppath = fs::temp_directory_path();
    config.spin_path = fs::absolute(config.spin_path);
    config.swarm_path = fs::absolute(config.swarm_path);

    // Main
    // spdlog::set_level(spdlog::level::debug);
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    // Create a Window
    Window window(1280, 720, "ImSpin");

    if (NFD_Init() != NFD_OKAY)
    {
        spdlog::error("Could not load NFD, you cannot open files");
    }

    ImGuiIO &io = ImGui::GetIO();
    ImGuiStyle &style = ImGui::GetStyle();

    // Variables for custom styles
    const char *styles[] = {"VioletDark", "Dark", "Light"};
    int current_style = 0;

    // Filepath variables
    std::string filepath = "example.pml";
    std::string filecontent;
    bool fileOpened = false, fileSaved = false, fileFormat = false;
    bool window_open = true;
    TextEditor edit;

    edit.SetLanguageDefinition(createPromelaLanguage());

    BasicStyling();
    CatppuccinMocha();
    RebuildFonts(100);
    ImGui::SetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags_TryToAvoidRefresh);

    double lastTimeSyntax = glfwGetTime();
    int drawingFrameCounter = 0;
    bool syntaxChecked = false;

    static char spin_path[256];
    strncpy(spin_path, config.spin_path.c_str(), config.spin_path.size());
    static char swarm_path[256];
    strncpy(swarm_path, config.swarm_path.c_str(), config.swarm_path.size());

    while (!window.shouldClose())
    {
        window.pollEvents();
        if (!ImGui::IsAnyMouseDown() && drawingFrameCounter > 5)
        {
            glfwWaitEventsTimeout(1);
            drawingFrameCounter = 0;
        }
        drawingFrameCounter++;
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING
        io.FontGlobalScale = config.global_scale * ImGui::GetWindowDpiScale();
        if (!window.isFocused)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
            continue;
        }

        if (fileOpened)
        {
            spdlog::info("Trying to open file {}", filepath);
            std::ifstream file(filepath);
            if (!file)
            {
                spdlog::error("Couldn't open file {}", filepath);
            }
            else
            {
                std::stringstream buffer;
                buffer << file.rdbuf();
                filecontent = buffer.str();

                spdlog::info("Setting text in editor");
                edit.SetText(filecontent);
            }
        }

        if (fileFormat) // Maybe do not use this because it is weird
        {
            // TODO: Patch Editor with pop_back();
            edit.SetText(spin::formatFile(edit.GetText()));
        }

        if (edit.IsTextChanged())
        {
            lastTimeSyntax = time;
            syntaxChecked = false;
        }

        if ((time - lastTimeSyntax) > config.syntaxCheckDelay && !syntaxChecked)
        {
            syntaxChecked = true;

            edit.SetErrorMarkers(spin::generateErrorMarkers(edit.GetText()));
        }

        window.startDrawing();

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

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("TabWindow", &window_open, window_flags);
        ImGui::PopStyleVar(1);
        RenderMenuBar(filepath, fileOpened, fileSaved, fileFormat);

        if (ImGui::BeginTabBar("Tabs"))
        {
            if (ImGui::BeginTabItem("Edit/View"))
            {
                RenderEditView(edit);
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Simulate/Replay"))
            {
                RenderSimulationView(edit);
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
                if (ImGui::DragFloat("Scale", &config.global_scale, 0.005f, 0.04, 1.0, "%.2f",
                                     ImGuiSliderFlags_AlwaysClamp))
                {
                    io.FontGlobalScale = config.global_scale * ImGui::GetWindowDpiScale();
                }

                ImGui::DragFloat("Syntax Check Delay", &config.syntaxCheckDelay, 0.1f, 0.1, 3.0, "%.2f",
                                 ImGuiSliderFlags_AlwaysClamp);

                if (ImGui::Combo("Styles", &current_style, styles, IM_ARRAYSIZE(styles)))
                {
                    switch (current_style)
                    {
                    case 0:
                        CatppuccinMocha();
                        break;
                    case 1:
                        ImGui::StyleColorsDark();
                        break;
                    case 2:
                        ImGui::StyleColorsLight();
                        break;
                    }
                }
                // SPIN
                if (ImGui::Button("Search Spin..."))
                {
                    fileDialogExe(config.spin_path);
                    strncpy(spin_path, config.spin_path.c_str(), 256);
                }
                ImGui::SameLine();
                if (ImGui::InputText("Spin Path:", spin_path, 256))
                {
                    config.spin_path = spin_path;
                }
                // SWARM
                if (ImGui::Button("Search Swarm..."))
                {
                    fileDialogExe(config.swarm_path);
                    strncpy(swarm_path, config.swarm_path.c_str(), 256);
                }
                ImGui::SameLine();
                if (ImGui::InputText("Swarm Path:", swarm_path, 256))
                {
                    config.swarm_path = swarm_path;
                }

                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }

        ImGui::End();
        ImGui::PopStyleVar(2);
        /* ImGui::ShowStyleEditor(); */
        /* ImGui::ShowDemoWindow(); */
        window.endDrawing();

        // END RENDERING
    }

    NFD_Quit();
    {
        spdlog::info("Writing Config");
        std::ofstream file("config.json");
        nlohmann::json j = config;
        if (file)
        {
            file << j;
            file.close();
        }
    }
    return EXIT_SUCCESS;
}
