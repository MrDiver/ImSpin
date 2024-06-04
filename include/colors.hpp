#pragma once
#include <imgui.h>

#define CATPPUCCIN_MOCHA_BASE00 ImVec4(32 / 255.0f, 33 / 255.0f, 52 / 255.0f, 1.0f)       // #202134
#define CATPPUCCIN_MOCHA_BASE01 ImVec4(50 / 255.0f, 52 / 255.0f, 70 / 255.0f, 1.0f)       // #323446
#define CATPPUCCIN_MOCHA_BASE02 ImVec4(68 / 255.0f, 70 / 255.0f, 89 / 255.0f, 1.0f)       // #444659
#define CATPPUCCIN_MOCHA_BASE03 ImVec4(80 / 255.0f, 82 / 255.0f, 107 / 255.0f, 1.0f)      // #50526B
#define CATPPUCCIN_MOCHA_BASE04 ImVec4(97 / 255.0f, 99 / 255.0f, 125 / 255.0f, 1.0f)      // #61637D
#define CATPPUCCIN_MOCHA_BASE05 ImVec4(138 / 255.0f, 141 / 255.0f, 206 / 255.0f, 1.0f)    // #8A8DCE
#define CATPPUCCIN_MOCHA_BASE06 ImVec4(198 / 255.0f, 208 / 255.0f, 245 / 255.0f, 1.0f)    // #C6D0F5
#define CATPPUCCIN_MOCHA_RED ImVec4(210 / 255.0f, 15 / 255.0f, 57 / 255.0f, 1.0f)         // #D20F39
#define CATPPUCCIN_MOCHA_GREEN ImVec4(64 / 255.0f, 168 / 255.0f, 95 / 255.0f, 1.0f)       // #40A85F
#define CATPPUCCIN_MOCHA_YELLOW ImVec4(251 / 255.0f, 185 / 255.0f, 46 / 255.0f, 1.0f)     // #FBB92E
#define CATPPUCCIN_MOCHA_BLUE ImVec4(90 / 255.0f, 149 / 255.0f, 250 / 255.0f, 1.0f)       // #5A95FA
#define CATPPUCCIN_MOCHA_MAGENTA ImVec4(188 / 255.0f, 97 / 255.0f, 217 / 255.0f, 1.0f)    // #BC61D9
#define CATPPUCCIN_MOCHA_CYAN ImVec4(78 / 255.0f, 183 / 255.0f, 187 / 255.0f, 1.0f)       // #4EB7BB
#define CATPPUCCIN_MOCHA_WHITE ImVec4(245 / 255.0f, 245 / 255.0f, 245 / 255.0f, 1.0f)     // #F5F5F5
#define CATPPUCCIN_MOCHA_ORANGE ImVec4(251 / 255.0f, 135 / 255.0f, 97 / 255.0f, 1.0f)     // #FB8761

#define CATPPUCCIN_MOCHA_ROSEWATER ImVec4(245 / 255.0f, 224 / 255.0f, 220 / 255.0f, 1.0f) // #f5e0dc
#define CATPPUCCIN_MOCHA_FLAMINGO ImVec4(242 / 255.0f, 205 / 255.0f, 205 / 255.0f, 1.0f)  // #f2cdcd
#define CATPPUCCIN_MOCHA_PINK ImVec4(245 / 255.0f, 194 / 255.0f, 231 / 255.0f, 1.0f)      // #f5c2e7
#define CATPPUCCIN_MOCHA_MAUVE ImVec4(203 / 255.0f, 166 / 255.0f, 247 / 255.0f, 1.0f)     // #cba6f7
// #define CATPPUCCIN_MOCHA_RED ImVec4(243 / 255.0f, 139 / 255.0f, 168 / 255.0f, 1.0f)       // #f38ba8
#define CATPPUCCIN_MOCHA_MAROON ImVec4(235 / 255.0f, 160 / 255.0f, 172 / 255.0f, 1.0f) // #eba0ac
#define CATPPUCCIN_MOCHA_PEACH ImVec4(250 / 255.0f, 179 / 255.0f, 135 / 255.0f, 1.0f)  // #fab387
// #define CATPPUCCIN_MOCHA_YELLOW ImVec4(249 / 255.0f, 226 / 255.0f, 175 / 255.0f, 1.0f)    // #f9e2af
// #define CATPPUCCIN_MOCHA_GREEN ImVec4(166 / 255.0f, 227 / 255.0f, 161 / 255.0f, 1.0f)     // #a6e3a1
#define CATPPUCCIN_MOCHA_TEAL ImVec4(148 / 255.0f, 226 / 255.0f, 213 / 255.0f, 1.0f)     // #94e2d5
#define CATPPUCCIN_MOCHA_SKY ImVec4(137 / 255.0f, 220 / 255.0f, 235 / 255.0f, 1.0f)      // #89dceb
#define CATPPUCCIN_MOCHA_SAPPHIRE ImVec4(116 / 255.0f, 199 / 255.0f, 236 / 255.0f, 1.0f) // #74c7ec
// #define CATPPUCCIN_MOCHA_BLUE ImVec4(137 / 255.0f, 180 / 255.0f, 250 / 255.0f, 1.0f)      // #89b4fa
#define CATPPUCCIN_MOCHA_LAVENDER ImVec4(180 / 255.0f, 190 / 255.0f, 254 / 255.0f, 1.0f) // #b4befe
#define CATPPUCCIN_MOCHA_TEXT ImVec4(205 / 255.0f, 214 / 255.0f, 244 / 255.0f, 1.0f)     // #cdd6f4
#define CATPPUCCIN_MOCHA_SUBTEXT1 ImVec4(186 / 255.0f, 194 / 255.0f, 222 / 255.0f, 1.0f) // #bac2de
#define CATPPUCCIN_MOCHA_SUBTEXT0 ImVec4(166 / 255.0f, 173 / 255.0f, 200 / 255.0f, 1.0f) // #a6adc8
#define CATPPUCCIN_MOCHA_OVERLAY2 ImVec4(147 / 255.0f, 153 / 255.0f, 178 / 255.0f, 1.0f) // #9399b2
#define CATPPUCCIN_MOCHA_OVERLAY1 ImVec4(127 / 255.0f, 132 / 255.0f, 156 / 255.0f, 1.0f) // #7f849c
#define CATPPUCCIN_MOCHA_OVERLAY0 ImVec4(108 / 255.0f, 112 / 255.0f, 134 / 255.0f, 1.0f) // #6c7086
#define CATPPUCCIN_MOCHA_SURFACE2 ImVec4(88 / 255.0f, 91 / 255.0f, 112 / 255.0f, 1.0f)   // #585b70
#define CATPPUCCIN_MOCHA_SURFACE1 ImVec4(69 / 255.0f, 71 / 255.0f, 90 / 255.0f, 1.0f)    // #45475a
#define CATPPUCCIN_MOCHA_SURFACE0 ImVec4(49 / 255.0f, 50 / 255.0f, 68 / 255.0f, 1.0f)    // #313244
#define CATPPUCCIN_MOCHA_BASE ImVec4(30 / 255.0f, 30 / 255.0f, 46 / 255.0f, 1.0f)        // #1e1e2e
#define CATPPUCCIN_MOCHA_MANTLE ImVec4(24 / 255.0f, 24 / 255.0f, 37 / 255.0f, 1.0f)      // #181825
#define CATPPUCCIN_MOCHA_CRUST ImVec4(17 / 255.0f, 17 / 255.0f, 27 / 255.0f, 1.0f)       // #11111b

// Text colors
#define IMGUI_TEXT CATPPUCCIN_MOCHA_BASE06
#define IMGUI_TEXT_DISABLED CATPPUCCIN_MOCHA_BASE05

// Background colors
#define IMGUI_WINDOW_BG CATPPUCCIN_MOCHA_BASE01
#define IMGUI_CHILD_BG CATPPUCCIN_MOCHA_BASE00
#define IMGUI_POPUP_BG CATPPUCCIN_MOCHA_BASE01

// Border colors
#define IMGUI_BORDER CATPPUCCIN_MOCHA_BASE04
#define IMGUI_BORDER_SHADOW IM_COL32(0, 0, 0, 0) // Transparent

// Frame BG colors
#define IMGUI_FRAME_BG CATPPUCCIN_MOCHA_BASE02
#define IMGUI_FRAME_BG_HOVERED CATPPUCCIN_MOCHA_BASE03
#define IMGUI_FRAME_BG_ACTIVE CATPPUCCIN_MOCHA_BASE04

// Title BG colors
#define IMGUI_TITLE_BG CATPPUCCIN_MOCHA_BASE01
#define IMGUI_TITLE_BG_ACTIVE CATPPUCCIN_MOCHA_BASE03
#define IMGUI_TITLE_BG_COLLAPSED CATPPUCCIN_MOCHA_BASE00

// Scrollbar colors
#define IMGUI_SCROLLBAR_BG CATPPUCCIN_MOCHA_BASE01
#define IMGUI_SCROLLBAR_GRAB CATPPUCCIN_MOCHA_BASE03
#define IMGUI_SCROLLBAR_GRAB_HOVERED CATPPUCCIN_MOCHA_BASE04
#define IMGUI_SCROLLBAR_GRAB_ACTIVE CATPPUCCIN_MOCHA_BASE05

// Checkmark, Slider, and Button colors
#define IMGUI_CHECK_MARK CATPPUCCIN_MOCHA_GREEN
#define IMGUI_SLIDER_GRAB CATPPUCCIN_MOCHA_BLUE
#define IMGUI_SLIDER_GRAB_ACTIVE CATPPUCCIN_MOCHA_CYAN

#define IMGUI_BUTTON CATPPUCCIN_MOCHA_BASE02
#define IMGUI_BUTTON_HOVERED CATPPUCCIN_MOCHA_BASE03
#define IMGUI_BUTTON_ACTIVE CATPPUCCIN_MOCHA_BASE04

// Header colors
#define IMGUI_HEADER CATPPUCCIN_MOCHA_BASE03
#define IMGUI_HEADER_HOVERED CATPPUCCIN_MOCHA_BASE04
#define IMGUI_HEADER_ACTIVE CATPPUCCIN_MOCHA_BASE05

// Tab colors
#define IMGUI_TAB CATPPUCCIN_MOCHA_BASE02
#define IMGUI_TAB_HOVERED CATPPUCCIN_MOCHA_BASE03
#define IMGUI_TAB_ACTIVE CATPPUCCIN_MOCHA_BASE04
#define IMGUI_TAB_UNFOCUSED CATPPUCCIN_MOCHA_BASE01
#define IMGUI_TAB_UNFOCUSED_ACTIVE CATPPUCCIN_MOCHA_BASE02

// Plot colors
#define IMGUI_PLOT_LINES CATPPUCCIN_MOCHA_BLUE
#define IMGUI_PLOT_LINES_HOVERED CATPPUCCIN_MOCHA_CYAN
#define IMGUI_PLOT_HISTOGRAM CATPPUCCIN_MOCHA_GREEN
#define IMGUI_PLOT_HISTOGRAM_HOVERED CATPPUCCIN_MOCHA_YELLOW

// Text selected and drag drop colors
#define IMGUI_TEXT_SELECTED_BG CATPPUCCIN_MOCHA_BASE03
#define IMGUI_DRAG_DROP_TARGET CATPPUCCIN_MOCHA_BLUE

// Nav highlight colors
#define IMGUI_NAV_HIGHLIGHT CATPPUCCIN_MOCHA_BLUE
#define IMGUI_NAV_WINDOWING_HIGHLIGHT CATPPUCCIN_MOCHA_CYAN
#define IMGUI_NAV_WINDOWING_DIM_BG IM_COL32(32, 33, 52, 128) // Semi-transparent

// Modal window colors
#define IMGUI_MODAL_WINDOW_DIM_BG IM_COL32(32, 33, 52, 192) // Semi-transparent

#define SYNTAX_KEYWORD CATPPUCCIN_MOCHA_MAGENTA
#define SYNTAX_STRING CATPPUCCIN_MOCHA_GREEN
#define SYNTAX_SYMBOL CATPPUCCIN_MOCHA_RED
#define SYNTAX_ESCAPE CATPPUCCIN_MOCHA_MAGENTA
#define SYNTAX_COMMENT CATPPUCCIN_MOCHA_BASE04
#define SYNTAX_CONSTANT CATPPUCCIN_MOCHA_ORANGE
#define SYNTAX_OPERATOR CATPPUCCIN_MOCHA_BLUE
#define SYNTAX_BRACES CATPPUCCIN_MOCHA_BASE04
#define SYNTAX_FUNCTION CATPPUCCIN_MOCHA_BLUE
#define SYNTAX_PARAMETER CATPPUCCIN_MOCHA_RED
#define SYNTAX_CLASS CATPPUCCIN_MOCHA_YELLOW
#define SYNTAX_PROPERTY CATPPUCCIN_MOCHA_BLUE
#define SYNTAX_ATTRIBUTE CATPPUCCIN_MOCHA_YELLOW

inline void CatppuccinMocha()
{
    auto &style                            = ImGui::GetStyle();
    ImVec4 *colors                         = style.Colors;
    colors[ImGuiCol_Text]                  = ImVec4(0.79f, 0.83f, 0.96f, 1.00f);
    colors[ImGuiCol_TextDisabled]          = ImVec4(0.43f, 0.45f, 0.55f, 1.00f);
    colors[ImGuiCol_WindowBg]              = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_ChildBg]               = ImVec4(0.12f, 0.13f, 0.19f, 1.00f);
    colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    colors[ImGuiCol_Border]                = ImVec4(0.41f, 0.22f, 0.44f, 0.27f);
    colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_FrameBg]               = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_FrameBgActive]         = ImVec4(0.71f, 0.75f, 1.00f, 0.99f);
    colors[ImGuiCol_TitleBg]               = ImVec4(0.12f, 0.12f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgActive]         = ImVec4(0.12f, 0.12f, 0.19f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_MenuBarBg]             = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.80f, 0.65f, 0.97f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.85f, 0.42f, 0.54f, 1.00f);
    colors[ImGuiCol_CheckMark]             = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_SliderGrab]            = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.85f, 0.42f, 0.54f, 1.00f);
    colors[ImGuiCol_Button]                = ImVec4(0.19f, 0.20f, 0.27f, 1.00f);
    colors[ImGuiCol_ButtonHovered]         = ImVec4(0.27f, 0.28f, 0.35f, 1.00f);
    colors[ImGuiCol_ButtonActive]          = ImVec4(0.35f, 0.36f, 0.44f, 1.00f);
    colors[ImGuiCol_Header]                = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_HeaderHovered]         = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_HeaderActive]          = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_Separator]             = ImVec4(0.41f, 0.22f, 0.44f, 1.00f);
    colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_SeparatorActive]       = ImVec4(0.94f, 0.37f, 0.53f, 1.00f);
    colors[ImGuiCol_ResizeGrip]            = ImVec4(0.80f, 0.65f, 0.97f, 1.00f);
    colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.95f, 0.55f, 0.66f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.85f, 0.42f, 0.54f, 1.00f);
    colors[ImGuiCol_Tab]                   = ImVec4(0.12f, 0.12f, 0.18f, 1.00f);
    colors[ImGuiCol_TabHovered]            = ImVec4(0.20f, 0.20f, 0.35f, 1.00f);
    colors[ImGuiCol_TabActive]             = ImVec4(0.41f, 0.21f, 0.44f, 1.00f);
    colors[ImGuiCol_TabUnfocused]          = ImVec4(0.20f, 0.20f, 0.35f, 1.00f);
    colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.41f, 0.22f, 0.44f, 1.00f);
    colors[ImGuiCol_DockingPreview]        = ImVec4(0.94f, 0.36f, 0.53f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_PlotLines]             = ImVec4(0.89f, 0.77f, 1.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.85f, 0.24f, 0.34f, 1.00f);
    colors[ImGuiCol_PlotHistogram]         = ImVec4(0.98f, 0.70f, 0.53f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.80f, 0.51f, 0.33f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.12f, 0.12f, 0.19f, 1.00f);
    colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.07f, 0.07f, 0.11f, 1.00f);
    colors[ImGuiCol_TableBorderLight]      = ImVec4(0.16f, 0.16f, 0.24f, 1.00f);
    colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
    colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.56f, 0.27f, 0.48f, 0.35f);
    colors[ImGuiCol_DragDropTarget]        = ImVec4(0.75f, 1.00f, 0.57f, 1.00f);
    colors[ImGuiCol_NavHighlight]          = ImVec4(0.94f, 0.36f, 0.53f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
}
