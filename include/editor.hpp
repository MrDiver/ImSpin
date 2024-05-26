#pragma once
#include <cstdint>
#include <imgui.h>
#include <string>
#include <vector>

enum EditMode
{
    Normal,
    Insert,
    Visual,
};

class CodeEditor
{
  public:
    CodeEditor(std::string name);
    void Render();
    // void setContent(std::string content);

  private:
    double blinktime = 0;
    bool blink = false;
    bool textChanged = true;
    ImFont *font;
    std::string name;
    std::vector<std::string> lines;
    uint32_t cursorX = 0;
    uint32_t cursorY = 0;
    EditMode mode = EditMode::Insert;

    void RenderLine(uint32_t n, const std::string &line);
    void resetBlink();
    void cursorDown(ImGuiKey key);
    void cursorUp(ImGuiKey key);
    void cursorLeft(ImGuiKey key);
    void cursorRight(ImGuiKey key);
    void moveCursorInBounds();
};
