#pragma once

#include <boost/circular_buffer.hpp>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <spdlog/sinks/base_sink.h>
#include <spinwrap.hpp>

namespace Shortcuts
{
inline ImGuiKeyChord OPEN   = (ImGuiKey_O | ImGuiMod_Ctrl);                // Ctrl+O
inline ImGuiKeyChord SAVE   = (ImGuiKey_S | ImGuiMod_Ctrl);                // Ctrl+S
inline ImGuiKeyChord EXIT   = (ImGuiKey_F4 | ImGuiMod_Alt);                // Alt+F4
inline ImGuiKeyChord FORMAT = (ImGuiKey_F | ImGuiMod_Alt | ImGuiMod_Ctrl); // Ctrl+Alt+F
} // namespace Shortcuts

template <typename Mutex> class ImGuiLogger : public spdlog::sinks::base_sink<Mutex>
{
  public:
    uint32_t max_size = 100;
    ImGuiLogger(uint32_t max_size = 100) : max_size(max_size), output(max_size)
    {
    }
    void Render()
    {
        for (std::string const &line : output)
        {
            ImGui::Text(line.c_str());
        }
        ImGui::SetScrollHereY(1.0f);
    }

  protected:
    void sink_it_(const spdlog::details::log_msg &msg) override
    {
        spdlog::memory_buf_t formatted;
        spdlog::sinks::base_sink<Mutex>::formatter_->format(msg, formatted);
        output.push_back(fmt::to_string(formatted));
        if (output.size() > max_size)
        {
            output.pop_front();
        }
    }

    void flush_() override
    {
    }

  private:
    boost::circular_buffer<std::string> output;
};

class SpinGui : public SpinWrap
{
  public:
    SpinGui();
    ~SpinGui();
    bool isRendering();
    void Render();
    void ProcessUpdates();

  private:
    // Attribtues
    bool needsRender;
    bool syntaxChecked = true;
    double lastTimeSyntax;
    TextEditor edit;
    fs::path currentFile;
    std::shared_ptr<ImGuiLogger<std::mutex>> spinlog;
    const char *styles[3] = {"Violet", "Dark", "Light"};

    // Parts
    void RenderMenuBar();
    void RenderTabs();
    void SetupDockEditView();
    void RenderEditView();
    void SetupDockSimulateView();
    void RenderSimulationView();
    // Utility
    bool FilePickerComponent(const char *label, const char *search_name, const char *filter, fs::path &path,
                             bool force_path = true);
    bool fileDialog(std::string &filepath, std::string const &title, std::string const &filter);
    // Style
    void BasicStyling();
    void RebuildFonts(float size);
    // Updating
    void triggerRender();
};
