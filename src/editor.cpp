#include "editor.hpp"
#include "colors.hpp"
#include "imgui.h"
#include <regex>
#include <spdlog/spdlog.h>
#include <unordered_set>

const std::unordered_set<std::string> promelaKeywords = {
    "active", "break",  "case",  "chan",    "do",     "od",    "if",     "fi",    "else",     "goto", "inline",
    "int",    "hidden", "local", "atomic",  "d_step", "mtype", "return", "short", "unsigned", "bool", "byte",
    "assert", "else",   "skip",  "timeout", "true",   "false", "eval",   "xr",    "xs"};

const std::unordered_set<char> promelaSymbols = {'{', '}', '(', ')', '[', ']', ';', ','};

struct Token
{
    std::string text;
    ImVec4 color;
};

std::vector<Token> TokenizePromela(const std::string &code)
{
    std::vector<Token> tokens;
    std::regex tokenRegex(R"((//.*?$)|(\s+|\w+|\W))", std::regex::multiline);
    auto wordsBegin = std::sregex_iterator(code.begin(), code.end(), tokenRegex);
    auto wordsEnd = std::sregex_iterator();

    for (auto it = wordsBegin; it != wordsEnd; ++it)
    {
        std::string word = it->str();
        ImVec4 color = CATPPUCCIN_MOCHA_WHITE; // Default color

        if (word.starts_with("//"))
        {
            color = SYNTAX_COMMENT; // Comment color
        }
        else if (promelaKeywords.find(word) != promelaKeywords.end())
        {
            color = SYNTAX_KEYWORD; // Keyword color
        }
        else if (word == "mtype")
        {
            color = SYNTAX_CLASS; // mtype color
        }
        else if (promelaSymbols.find(word[0]) != promelaSymbols.end())
        {
            color = SYNTAX_BRACES; // Symbol color
        }
        else if (std::isspace(word[0]))
        {
            color = CATPPUCCIN_MOCHA_WHITE; // Whitespace color
        }

        tokens.push_back({word, color});
    }

    return tokens;
}

void RenderPromelaCode(const std::string &code)
{
    std::vector<Token> tokens = TokenizePromela(code);
    for (const auto &token : tokens)
    {
        ImGui::TextColored(token.color, "%s", token.text.c_str());
        ImGui::SameLine(0, 0); // No spacing between tokens
    }
    ImGui::NewLine(); // Move to next line after rendering the tokens
}

void ReplaceTabsWithSpaces(std::vector<std::string> &lines, int spacesPerTab = 4)
{
    const std::string spaces(spacesPerTab, ' ');

    for (auto &line : lines)
    {
        size_t pos = 0;
        while ((pos = line.find('\t', pos)) != std::string::npos)
        {
            line.replace(pos, 1, spaces);
            pos += spacesPerTab;
        }
    }
}

ImU32 ImVec4ToIM_COL32(const ImVec4 &color)
{
    return IM_COL32(static_cast<int>(color.x * 255.0f), static_cast<int>(color.y * 255.0f),
                    static_cast<int>(color.z * 255.0f), static_cast<int>(color.w * 255.0f));
}

std::vector<std::string> SplitStringIntoLines(const std::string &input)
{
    std::vector<std::string> lines;
    std::istringstream stream(input);
    std::string line;

    while (std::getline(stream, line))
    {
        lines.push_back(line);
    }

    return lines;
}

void CodeEditor::resetBlink()
{
    this->blink = false;
    this->blinktime = 0;
}

CodeEditor::CodeEditor(std::string name) : name(name)
{
    const std::string promelaCode = R"(active proctype A() {
    int x;
    if
        :: x > 0 -> x = x - 1;
        :: else -> skip;
    fi
}
active proctype A() {
    int x;
    if
        :: x > 0 -> x = x - 1;
        :: else -> skip;
    fi
}
    )";
    lines = SplitStringIntoLines(promelaCode);

    ImGuiIO &io = ImGui::GetIO();
    this->font = io.Fonts->Fonts[1];

    ReplaceTabsWithSpaces(lines);
}

void CodeEditor::Render()
{

    // Handle Text Input
    if (this->mode == Insert)
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            lines.insert(lines.begin() + cursorY + 1, "");
            cursorY++;
            cursorX = 0;
            resetBlink();
        }
        else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)) && cursorX > 0)
        {
            lines[cursorY].erase(cursorX - 1, 1);
            cursorX--;
            resetBlink();
        }
        else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)) && cursorX < lines[cursorY].size())
        {
            lines[cursorY].erase(cursorX, 1);
            resetBlink();
        }
        else if (ImGui::GetIO().InputQueueCharacters.Size > 0)
        {
            char c = ImGui::GetIO().InputQueueCharacters[0];
            lines[cursorY].insert(cursorX, 1, c);
            cursorX++;
            ImGui::GetIO().InputQueueCharacters.clear();
            resetBlink();
        }
    }

    // Handle Movement and Modes
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_UpArrow)) && cursorY > 0)
    {
        cursorY--;
        resetBlink();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_DownArrow)) && cursorY < lines.size() - 1)
    {
        cursorY++;
        resetBlink();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_LeftArrow)) && cursorX > 0)
    {
        cursorX--;
        resetBlink();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_RightArrow)) && cursorX < lines[cursorY].size())
    {
        cursorX++;
        resetBlink();
    }
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
    {
        this->mode = Normal;
    }

    if (this->mode == Normal)
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_K)) && cursorY > 0)
        {
            cursorY--;
            resetBlink();
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_J)) && cursorY < lines.size() - 1)
        {
            cursorY++;
            resetBlink();
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_H)) && cursorX > 0)
        {
            cursorX--;
            resetBlink();
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_L)) && cursorX < lines[cursorY].size())
        {
            cursorX++;
            resetBlink();
        }
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_I)))
        {
            this->mode = Insert;
        }
    }

    ImGui::Begin("Editor");
    ImGui::PushFont(this->font);
    uint32_t n = 0;
    for (const std::string &line : this->lines)
    {
        RenderLine(n++, line);
    }
    ImGui::PopFont();
    ImGui::End();
}

void CodeEditor::RenderLine(uint32_t n, const std::string &line)
{
    if (ImGui::GetTime() - blinktime > 0.5)
    {
        blink = !blink;
        blinktime = ImGui::GetTime();
    }
    static float numberlinewidth =
        ImGui::CalcTextSize("0000").x; // TODO: Could be calculated with log_10 of the lines of the file
    ImGui::TextColored(n == cursorY ? CATPPUCCIN_MOCHA_LAVENDER : CATPPUCCIN_MOCHA_OVERLAY1, "%3i", n);
    ImGui::SameLine();
    // ImGui::TextColored(ImVec4(1, 1, 1, 1), "%s", line.c_str());
    RenderPromelaCode(line);
    if (n == cursorY && blink)
    {
        ImVec2 textPos = ImGui::GetCursorScreenPos();
        ImVec2 cursorPos = textPos;
        cursorPos.x += ImGui::CalcTextSize(line.substr(0, cursorX).c_str()).x;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddLine(ImVec2(cursorPos.x + numberlinewidth, cursorPos.y - 10),
                           ImVec2(cursorPos.x + numberlinewidth, cursorPos.y - ImGui::GetTextLineHeight()),
                           ImVec4ToIM_COL32(this->mode == Insert ? CATPPUCCIN_MOCHA_ROSEWATER : CATPPUCCIN_MOCHA_RED),
                           3);
        // TODO: Scroll window when cursor goes outside visible region
    }
}
