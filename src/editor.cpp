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

    io = ImGui::GetIO();
    style = ImGui::GetStyle();

    if (io.Fonts->Fonts.size() > 1)
    {
        this->font = io.Fonts->Fonts[1];
    }

    ReplaceTabsWithSpaces(lines);
}

void CodeEditor::cursorUp(ImGuiKey key)
{
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(key)) && cursorY > 0)
    {
        cursorY--;
        resetBlink();
        updateScroll = true;
    }
}

void CodeEditor::cursorDown(ImGuiKey key)
{
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(key)) && cursorY < lines.size() - 1)
    {
        cursorY++;
        resetBlink();
        updateScroll = true;
    }
}

void CodeEditor::cursorLeft(ImGuiKey key)
{
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(key)) && cursorX > 0)
    {
        moveCursorInBounds();
        cursorX--;
        resetBlink();
        updateScroll = true;
    }
}

void CodeEditor::cursorRight(ImGuiKey key)
{
    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(key)) && cursorX < lines[cursorY].size())
    {
        cursorX++;
        resetBlink();
        updateScroll = true;
    }
}

void CodeEditor::moveCursorInBounds()
{
    if (cursorX >= lines[cursorY].size())
    {
        cursorX = lines[cursorY].size();
    }
}

void CodeEditor::RebuildTokens()
{
    this->tokens.clear();

#ifdef _WIN32
    std::regex tokenRegex(R"((\s+|\w+|\W))");
#else
    std::regex tokenRegex(R"((\s+|\w+|\W))", std::regex::multiline);
#endif
    for (std::string const &line : lines)
    {
        auto wordsBegin = std::sregex_iterator(line.begin(), line.end(), tokenRegex);
        auto wordsEnd = std::sregex_iterator();

        std::vector<Token> lineTokens;

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

            lineTokens.push_back({word, color});
        }
        auto endLine = tokens.size();
        this->tokens.push_back(lineTokens);
    }
}

void CodeEditor::changed()
{
    this->textChanged = true;
}

void CodeEditor::scroll()
{
    if (updateScroll)
    {
        auto y = ImGui::GetScrollY();

        y += 0.1 * (scrollTarget.y - y);

        if (fabs(lastScroll.y - y) < 0.01)
        {
            updateScroll = false;
            lastScroll.y = scrollTarget.y;
        }
        else
        {
            lastScroll.y = y;
            lastScroll.x = ImGui::GetScrollX();
        }
    }
}

void CodeEditor::Render()
{
    if (updateScroll)
    {
        ImGui::SetNextWindowScroll(lastScroll);
    }

    ImGui::Begin("Editor");
    if (this->font)
    {
        ImGui::PushFont(this->font);
    }
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));

    if (textChanged)
    {
        this->RebuildTokens();
        textChanged = false;
    }

    scroll();

    static ImGuiIO &io = ImGui::GetIO();
    // Handle Text Input
    if (this->mode == Insert)
    {
        if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter)))
        {
            lines.insert(lines.begin() + cursorY + 1, "");
            cursorY++;
            cursorX = 0;
            resetBlink();
            changed();
            updateScroll = true;
        }
        else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Backspace)) && cursorX > 0)
        {
            moveCursorInBounds();
            lines[cursorY].erase(cursorX - 1, 1);
            cursorX--;
            resetBlink();
            changed();
            updateScroll = true;
        }
        else if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)) && cursorX < lines[cursorY].size())
        {
            moveCursorInBounds();
            lines[cursorY].erase(cursorX, 1);
            resetBlink();
            changed();
            updateScroll = true;
        }
        else if (ImGui::GetIO().InputQueueCharacters.Size > 0)
        {
            char c = ImGui::GetIO().InputQueueCharacters[0];
            moveCursorInBounds();
            lines[cursorY].insert(cursorX, 1, c);
            cursorX++;
            io.InputQueueCharacters.clear();
            resetBlink();
            changed();
            updateScroll = true;
        }
    }

    // Handle Movement and Modes
    cursorUp(ImGuiKey_UpArrow);
    cursorDown(ImGuiKey_DownArrow);
    cursorLeft(ImGuiKey_LeftArrow);
    cursorRight(ImGuiKey_RightArrow);

    if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape)))
    {
        this->mode = Normal;
    }

    if (this->mode == Normal && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_I)))
    {
        this->mode = Insert;
    }

    if (this->mode == Normal)
    {
        cursorUp(ImGuiKey_K);
        cursorDown(ImGuiKey_J);
        cursorLeft(ImGuiKey_H);
        cursorRight(ImGuiKey_L);
    }

    uint32_t n = 0;
    for (const std::string &line : this->lines)
    {
        RenderLine(n++, line);
    }
    if (this->font)
    {
        ImGui::PopFont();
    }
    ImGui::PopStyleVar(2);
    ImGui::End();
}

void CodeEditor::RenderLine(uint32_t n, const std::string &line)
{
    static ImDrawList *drawList = ImGui::GetWindowDrawList();
    float numberlinewidth =
        ImGui::CalcTextSize("0000").x;            // TODO: Could be calculated with log_10 of the lines of the file
    ImVec2 textPos = ImGui::GetCursorScreenPos(); // Getting the current position where the text ends for this line
    ImVec2 cursorPos = textPos;

    if (ImGui::GetTime() - blinktime > 0.5)
    {
        blink = !blink;
        blinktime = ImGui::GetTime();
    }
    if (n == cursorY)
    {
        cursorPos.x += ImGui::CalcTextSize(line.substr(0, cursorX).c_str()).x;

        ImVec2 rectMin = ImVec2(0, textPos.y);
        ImVec2 rectMax = ImVec2(0 + ImGui::GetContentRegionAvail().x, textPos.y + ImGui::GetTextLineHeight());
        drawList->AddRectFilled(rectMin, rectMax,
                                ImVec4ToIM_COL32(CATPPUCCIN_MOCHA_TEXT) &
                                    (0x11000000 | ~0xFF000000)); // Background color for the current line

        auto winsize = ImGui::GetWindowSize();
        auto scroll = ImVec2(ImGui::GetScrollY(), ImGui::GetScrollMaxY());
        auto pad = 3 * ImGui::GetTextLineHeight();
        auto wintop = scroll.x + pad;
        auto winbot = scroll.x + winsize.y - pad;
        auto ypos = ImGui::GetCursorPosY();

        if (ypos < wintop)
        {
            scrollTarget = ImVec2(0, ypos - pad);
        }
        else if (ypos > winbot)
        {
            scrollTarget = ImVec2(0, ypos - winsize.y + pad);
        }
        else
        {
            scrollTarget = ImVec2(0, scroll.x);
        }
    }

    ImGui::TextColored(n == cursorY ? CATPPUCCIN_MOCHA_LAVENDER : CATPPUCCIN_MOCHA_OVERLAY1, "%3i ", n);
    ImGui::SameLine();

    if (n < tokens.size())
    {
        for (auto const &line : tokens[n])
        {
            ImGui::TextColored(line.color, "%s", line.text.c_str());
            ImGui::SameLine(0, 0); // No spacing between tokens
        }
    }
    ImGui::NewLine(); // Move to next line after rendering the tokens

    if (n == cursorY && blink)
    {

        drawList->AddLine(ImVec2(cursorPos.x + numberlinewidth, cursorPos.y),
                          ImVec2(cursorPos.x + numberlinewidth, cursorPos.y + ImGui::GetTextLineHeight()),
                          ImVec4ToIM_COL32(this->mode == Insert ? CATPPUCCIN_MOCHA_ROSEWATER : CATPPUCCIN_MOCHA_RED),
                          10 * io.FontGlobalScale);
    }
}
