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

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
}

#define SHORTCUT_OPEN (ImGuiKey_O | ImGuiMod_Ctrl)     // Ctrl+O
#define SHORTCUT_SAVE (ImGuiKey_S | ImGuiMod_Ctrl)     // Ctrl+S
#define SHORTCUT_EXIT (ImGuiKey_Escape | ImGuiMod_Alt) // Alt+F4

void fileDialog(std::string &filepath)
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
        fileDialog(filepath);
        fileOpened = true;
    }
    if (ImGui::IsKeyChordPressed(ImGuiKey_F | ImGuiKey_LeftAlt | ImGuiKey_LeftCtrl))
    {
        fileFormat = true;
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {
                fileDialog(filepath);
                fileOpened = true;
            }

            if (ImGui::MenuItem("Format", "Ctrl+Alt+F"))
            {
                fileFormat = true;
            }
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

static bool TokenizeStyleString(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p = in_begin;

    if (*p == '"')
    {
        p++;

        while (p < in_end)
        {
            // handle end of string
            if (*p == '"')
            {
                out_begin = in_begin;
                out_end = p + 1;
                return true;
            }

            // handle escape character for "
            if (*p == '\\' && p + 1 < in_end && p[1] == '"')
                p++;

            p++;
        }
    }

    return false;
}

static bool TokenizeCharacterLiteral(const char *in_begin, const char *in_end, const char *&out_begin,
                                     const char *&out_end)
{
    const char *p = in_begin;

    if (*p == '\'')
    {
        p++;

        // handle escape characters
        if (p < in_end && *p == '\\')
            p++;

        if (p < in_end)
            p++;

        // handle end of character literal
        if (p < in_end && *p == '\'')
        {
            out_begin = in_begin;
            out_end = p + 1;
            return true;
        }
    }

    return false;
}

static bool TokenizeIdentifier(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p = in_begin;

    if ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || *p == '_')
    {
        p++;

        while ((p < in_end) &&
               ((*p >= 'a' && *p <= 'z') || (*p >= 'A' && *p <= 'Z') || (*p >= '0' && *p <= '9') || *p == '_'))
            p++;

        out_begin = in_begin;
        out_end = p;
        return true;
    }

    return false;
}

static bool TokenizeNumber(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    const char *p = in_begin;

    const bool startsWithNumber = *p >= '0' && *p <= '9';

    if (*p != '+' && *p != '-' && !startsWithNumber)
        return false;

    p++;

    bool hasNumber = startsWithNumber;

    while (p < in_end && (*p >= '0' && *p <= '9'))
    {
        hasNumber = true;

        p++;
    }

    if (hasNumber == false)
        return false;

    bool isFloat = false;
    bool isHex = false;
    bool isBinary = false;

    if (p < in_end)
    {
        if (*p == '.')
        {
            isFloat = true;

            p++;

            while (p < in_end && (*p >= '0' && *p <= '9'))
                p++;
        }
        else if (*p == 'x' || *p == 'X')
        {
            // hex formatted integer of the type 0xef80

            isHex = true;

            p++;

            while (p < in_end && ((*p >= '0' && *p <= '9') || (*p >= 'a' && *p <= 'f') || (*p >= 'A' && *p <= 'F')))
                p++;
        }
        else if (*p == 'b' || *p == 'B')
        {
            // binary formatted integer of the type 0b01011101

            isBinary = true;

            p++;

            while (p < in_end && (*p >= '0' && *p <= '1'))
                p++;
        }
    }

    if (isHex == false && isBinary == false)
    {
        // floating point exponent
        if (p < in_end && (*p == 'e' || *p == 'E'))
        {
            isFloat = true;

            p++;

            if (p < in_end && (*p == '+' || *p == '-'))
                p++;

            bool hasDigits = false;

            while (p < in_end && (*p >= '0' && *p <= '9'))
            {
                hasDigits = true;

                p++;
            }

            if (hasDigits == false)
                return false;
        }

        // single precision floating point type
        if (p < in_end && *p == 'f')
            p++;
    }

    if (isFloat == false)
    {
        // integer size type
        while (p < in_end && (*p == 'u' || *p == 'U' || *p == 'l' || *p == 'L'))
            p++;
    }

    out_begin = in_begin;
    out_end = p;
    return true;
}

static bool TokenizePunctuation(const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end)
{
    (void)in_end;

    switch (*in_begin)
    {
    case '[':
    case ']':
    case '{':
    case '}':
    case '!':
    case '%':
    case '^':
    case '&':
    case '*':
    case '(':
    case ')':
    case '-':
    case '+':
    case '=':
    case '~':
    case '|':
    case '<':
    case '>':
    case '?':
    case ':':
    case '/':
    case ';':
    case ',':
    case '.':
        out_begin = in_begin;
        out_end = in_begin + 1;
        return true;
    }

    return false;
}

TextEditor::LanguageDefinition createPromelaLanguage()
{
    TextEditor::LanguageDefinition promela;
    promela.mName = "Promela";
    promela.mKeywords = {
        "active", "assert",   "atomic", "bool",   "break", "byte",   "case",    "chan",   "d_step",   "do",    "else",
        "else",   "eval",     "false",  "fi",     "goto",  "hidden", "if",      "inline", "int",      "local", "mtype",
        "od",     "proctype", "return", "select", "short", "skip",   "timeout", "true",   "unsigned", "xr",    "xs",
    };

    promela.mTokenize = [](const char *in_begin, const char *in_end, const char *&out_begin, const char *&out_end,
                           TextEditor::PaletteIndex &paletteIndex) -> bool {
        paletteIndex = TextEditor::PaletteIndex::Max;

        while (in_begin < in_end && isascii(*in_begin) && isblank(*in_begin))
            in_begin++;

        if (in_begin == in_end)
        {
            out_begin = in_end;
            out_end = in_end;
            paletteIndex = TextEditor::PaletteIndex::Default;
        }
        else if (TokenizeStyleString(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::String;
        else if (TokenizeCharacterLiteral(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::CharLiteral;
        else if (TokenizeIdentifier(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::Identifier;
        else if (TokenizeNumber(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::Number;
        else if (TokenizePunctuation(in_begin, in_end, out_begin, out_end))
            paletteIndex = TextEditor::PaletteIndex::Punctuation;

        return paletteIndex != TextEditor::PaletteIndex::Max;
    };
    promela.mCommentStart = "/*";
    promela.mCommentEnd = "*/";
    promela.mSingleLineComment = "//";
    return promela;
}

void formatProgram(std::string &filecontent)
{
}

int main()
{
    using namespace boost::process;
    using namespace boost::xpressive;

    // Reading Config
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
    bool fileOpened = true, fileSaved = false, fileFormat = false;
    bool window_open = true;
    TextEditor edit;

    edit.SetLanguageDefinition(createPromelaLanguage());

    BasicStyling();
    CatppuccinMocha();
    RebuildFonts(100);
    ImGui::SetNextWindowRefreshPolicy(ImGuiWindowRefreshFlags_TryToAvoidRefresh);

    double lastTimeSyntax = glfwGetTime();
    bool syntaxChecked = false;

    while (!window.shouldClose())
    {
        window.pollEvents();
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING
        io.FontGlobalScale = config.global_scale * ImGui::GetWindowDpiScale();
        if (!window.isFocused)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
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

        if (fileFormat)
        {
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
                if (ImGui::DragFloat("Scale", &config.global_scale, 0.005f, 0.04, 1.0, "%.2f",
                                     ImGuiSliderFlags_AlwaysClamp))
                {
                    io.FontGlobalScale = config.global_scale * ImGui::GetWindowDpiScale();
                }

                ImGui::DragFloat("Scale", &config.syntaxCheckDelay, 0.1f, 0.1, 3.0, "%.2f",
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
