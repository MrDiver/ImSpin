#include "spingui.hpp"
#include "GLFW/glfw3.h"
#include "colors.hpp"
#include "fonts.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "spinlang.hpp"
#include <imgui_stdlib.h>
#include <nfd.h>
#include <spdlog/spdlog.h>

SpinGui::SpinGui()
{
    spinlog = std::make_shared<ImGuiLogger<std::mutex>>();
    spdlog::default_logger()->sinks().push_back(spinlog);
    if (NFD_Init() != NFD_OKAY)
    {
        spdlog::error("Could not load NFD, you cannot open files");
    }
    edit.SetLanguageDefinition(createPromelaLanguage());
    // ImGui::SetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags_TryToAvoidRefresh);
    BasicStyling();
    RebuildFonts(100);
    readConfigFile();

    switch (getConfig().theme)
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

SpinGui::~SpinGui()
{
    NFD_Quit();
    writeConfigFile();
}

bool SpinGui::isRendering()
{
    return needsRender;
}

void SpinGui::ProcessUpdates()
{

    if (edit.IsTextChanged())
    {
        lastTimeSyntax = glfwGetTime();
        syntaxChecked  = false;
    }

    if ((glfwGetTime() - lastTimeSyntax) > getConfig().syntaxCheckDelay && !syntaxChecked)
    {
        syntaxChecked = true;

        edit.SetErrorMarkers(generateErrorMarkers(edit.GetText()));
    }
}

void SpinGui::Render()
{
    needsRender = false;
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
    ImGui::Begin("TabWindow", NULL, window_flags);
    ImGui::PopStyleVar(1);
    ImGui::GetIO().FontGlobalScale = getConfig().global_scale * ImGui::GetWindowDpiScale();

    RenderMenuBar();
    RenderTabs();
    //
    ImGui::End();
    ImGui::PopStyleVar(2);
}

void SpinGui::RenderMenuBar()
{
    auto openModel = [&]() {
        std::string path;
        if (fileDialog(path, "Promela Model", "pml"))
        {
            currentFile         = path;
            std::string content = edit.GetText();

            if (readFile(currentFile, content))
            {
                edit.SetText(content);
            }
        }
        else
        {
            spdlog::warn("Could not open promela model, file does not exist");
        }
    };

    if (ImGui::IsKeyChordPressed(Shortcuts::OPEN))
    {
        openModel();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                openModel();
            }

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
}

void SpinGui::RenderTabs()
{
    if (ImGui::BeginTabBar("Tabs"))
    {
        if (ImGui::BeginTabItem("Edit/View"))
        {
            RenderEditView();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Simulate/Replay"))
        {
            RenderSimulationView();
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
            if (ImGui::DragFloat("Scale", &mutConfig().global_scale, 0.005f, 0.04, 1.0, "%.2f",
                                 ImGuiSliderFlags_AlwaysClamp))
            {
                ImGui::GetIO().FontGlobalScale = getConfig().global_scale * ImGui::GetWindowDpiScale();
            }

            ImGui::DragFloat("Syntax Check Delay", &mutConfig().syntaxCheckDelay, 0.1f, 0.1, 3.0, "%.2f",
                             ImGuiSliderFlags_AlwaysClamp);

            if (ImGui::Combo("Styles", &mutConfig().theme, styles, IM_ARRAYSIZE(styles)))
            {
                switch (getConfig().theme)
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
            FilePickerComponent("Search Spin...", "Spin Executable", "", mutConfig().spin_path);
            FilePickerComponent("Search Swarm...", "Swarm Executable", "", mutConfig().swarm_path);
            FilePickerComponent("Search Dot...", "Dot Executable", "", mutConfig().dot_path, false);
            FilePickerComponent("Search Compiler...", "Compiler Executable", "", mutConfig().compiler_path, false);

            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}

void SpinGui::SetupDockEditView()
{
    static bool editViewDockInitialized = false;
    if (!editViewDockInitialized)
    {
        static ImGuiID dockSpaceId = ImGui::GetID("EditViewDock");
        ImGuiID dockUp, dockDown, dockLeft, dockRight;

        ImGui::DockBuilderRemoveNode(dockSpaceId);
        ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetWindowSize());
        dockDown  = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down, 0.25, nullptr, &dockSpaceId);
        dockRight = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.25, nullptr, &dockSpaceId);
        ImGui::DockBuilderDockWindow("Editor", dockSpaceId);
        ImGui::DockBuilderDockWindow("Automata View", dockRight);
        ImGui::DockBuilderDockWindow("Log", dockDown);
        ImGui::DockBuilderFinish(dockSpaceId);
        editViewDockInitialized = true;
    }
}

void SpinGui::RenderEditView()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    SetupDockEditView();

    ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockSpaceId               = ImGui::GetID("EditViewDock");
    ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), dockSpaceFlags);

    ImGui::Begin("Editor");
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    edit.Render("promelathing");
    ImGui::PopFont();
    ImGui::End();

    // ImGui::SetNextWindowDockID(dockRight);
    ImGui::Begin("Automata View");
    ImGui::End();

    // ImGui::SetNextWindowDockID(dockDown);
    ImGui::Begin("Log");
    spinlog->Render();
    ImGui::End();

    ImGui::PopStyleVar(1);
}

void SpinGui::SetupDockSimulateView()
{
    static bool editViewDockInitialized = false;
    if (!editViewDockInitialized)
    {
        static ImGuiID dockSpaceId = ImGui::GetID("SimulateViewDock");
        ImGuiID dockUp, dockDown, dockLeft, dockRight;

        ImGui::DockBuilderRemoveNode(dockSpaceId);
        ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetWindowSize());
        dockDown  = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Down, 0.25, nullptr, &dockSpaceId);
        dockRight = ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.25, nullptr, &dockSpaceId);
        ImGui::DockBuilderDockWindow("Editor", dockSpaceId);
        ImGui::DockBuilderDockWindow("Automata View", dockRight);
        ImGui::DockBuilderDockWindow("Log", dockDown);
        ImGui::DockBuilderFinish(dockSpaceId);
        editViewDockInitialized = true;
    }
}

void SpinGui::RenderSimulationView()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    // SetupDockEditView();

    ImGuiDockNodeFlags dockSpaceFlags = ImGuiDockNodeFlags_None | ImGuiDockNodeFlags_PassthruCentralNode;
    ImGuiID dockSpaceId               = ImGui::GetID("SimulateViewDock");
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
                ImGui::RadioButton("Random with seed", (int *)&mutSettings().simulationMode, RANDOM);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputInt("##Seed", &mutSettings().seed);
                ImGui::PopItemWidth();
                ImGui::RadioButton("Interactive (for resolution of all nondeterminism)",
                                   (int *)&mutSettings().simulationMode, INTERACTIVE);
                ImGui::RadioButton("Guided, with trail: ", (int *)&mutSettings().simulationMode, GUIDED);
                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Browse").x - 60);
                FilePickerComponent("##TrailPath", "Trail", "trail", mutSettings().trailPath);
                ImGui::PopItemWidth();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputInt("Initial steps skipped", &mutSettings().initialStepsSkipped);
                ImGui::InputInt("Maximum number of steps", &mutSettings().maximumNumberOfSteps);
                ImGui::PopItemWidth();
                ImGui::Checkbox("Track Data Values (this can be slow)", &mutSettings().trackDataValues);
            }
            ImGui::TableNextColumn();
            {
                ImGui::Text("A Full Channel");
                ImGui::RadioButton("blocks new messages", (int *)&mutSettings().channelMode, BLOCKING);
                ImGui::RadioButton("loses new messages", (int *)&mutSettings().channelMode, LOSING);
                ImGui::Checkbox("MSC+stmnt", &mutSettings().MSCStmnt);
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("MSC max text width").x -
                                     20);
                ImGui::InputInt("MSC max text width", &mutSettings().MSCMaxTextWidth, 1, 1000);
                ImGui::InputInt("MSC update delay", &mutSettings().MSCUpdateDelay, 1, 1000);
                ImGui::PopItemWidth();
            }
            ImGui::TableNextColumn();
            {
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize("Tracked Variable").x - 20);
                ImGui::Text("Output Filtering [regex]");
                ImGui::InputText("Process Ids", &mutSettings().filterProcessIds);
                ImGui::InputText("Queue Ids", &mutSettings().filterQueueIds);
                ImGui::InputText("Var Names", &mutSettings().filterVarNames);
                ImGui::InputText("Tracked Variable", &mutSettings().filterTrackedVariable);
                ImGui::InputText("Track Scaling", &mutSettings().filterTrackScaling);
                ImGui::PopItemWidth();
            }
            ImGui::EndTable();
        }
    }
    ImGui::End();
    if (ImGui::Begin("Controls"))
    {
        int tmp = ImGui::GetContentRegionAvail().x;
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

    ImGui::Begin("Editor##Sim");
    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[1]);
    edit.Render("promelathing");
    ImGui::PopFont();
    ImGui::End();

    // ImGui::SetNextWindowDockID(dockDown);
    ImGui::Begin("Simulation Log");
    ImGui::End();

    ImGui::PopStyleVar(1);
}

bool SpinGui::FilePickerComponent(const char *label, const char *search_name, const char *filter, fs::path &path,
                                  bool force_path)
{
    std::string s_path;
    if (force_path)
    {
        s_path = fs::relative(path).string();
    }
    else
    {
        s_path = path.string();
    }
    bool opened = false;
    ImGui::PushID(label);
    if (ImGui::Button("Browse"))
    {
        if (fileDialog(s_path, search_name, filter))
        {
            path   = fs::absolute(s_path);
            opened = true;
        }
    }
    ImGui::SameLine();
    if (ImGui::InputText(label, &s_path))
    {
        path   = s_path;
        opened = true;
    }
    ImGui::PopID();
    return opened;
}

bool SpinGui::fileDialog(std::string &filepath, std::string const &title, std::string const &filter)
{
    char *outPath;
    nfdfilteritem_t filterItem[2] = {
        {title.c_str(), filter.c_str()},
    };
    nfdresult_t result = NFD_OpenDialog(&outPath, filterItem, filter.empty() ? 0 : 1, NULL);
    if (result == NFD_OKAY)
    {
        filepath = outPath;
        return true;
    }
    return false;
}

void SpinGui::BasicStyling()
{
    ImGuiStyle &style              = ImGui::GetStyle();
    style.WindowPadding            = ImVec2(20, 20);
    style.FramePadding             = ImVec2(20, 6);
    style.CellPadding              = ImVec2(20, 6);
    style.ItemSpacing              = ImVec2(20, 6);
    style.ItemInnerSpacing         = ImVec2(20, 0);
    style.GrabMinSize              = 20;
    style.TabRounding              = 4;
    style.FrameRounding            = 4;
    style.ScrollbarRounding        = 12;
    style.GrabRounding             = 3;
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.SeparatorTextBorderSize  = 2;
    style.SeparatorTextAlign       = ImVec2(0.5, 0.5);
    style.SeparatorTextPadding     = ImVec2(20, 18);
    style.FrameBorderSize          = 1;
    style.WindowBorderSize         = 1;
    style.PopupBorderSize          = 1;
}

void SpinGui::RebuildFonts(float size)
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

void SpinGui::triggerRender()
{
    needsRender = true;
}
