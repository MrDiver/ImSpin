#include "imgui_internal.h"
#include "spdlog/logger.h"
#define GLAD_GL_IMPLEMENTATION
#define GLFW_INCLUDE_NONE
#include <boost/process.hpp>
#include <boost/xpressive/xpressive.hpp>
#include <glad/gl.h>
// GLAD BEFORE GLFW
#include <GLFW/glfw3.h>
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
#include "spingui.hpp"
#include "spinlang.hpp"

// FIXME: Make flag to not overwrite layout when imgui.config exists

void error_callback(int32_t error, const char *description)
{
    spdlog::error(description);
}

// std::vector<std::string> getProgramFilesPaths()
// {
//     std::vector<std::string> paths;
//     char *programFiles    = getenv("ProgramFiles");
//     char *programFilesX86 = getenv("ProgramFiles(x86)");
//
//     if (programFiles)
//     {
//         paths.push_back(std::string(programFiles));
//     }
//     if (programFilesX86)
//     {
//         paths.push_back(std::string(programFilesX86));
//     }
//
//     return paths;
// }
//
// std::string locateVSExecutable(std::string filename)
// {
//     namespace fs                               = std::filesystem;
//     std::vector<std::string> programFilesPaths = getProgramFilesPaths();
//     std::string relativePath                   = "\\Microsoft Visual Studio\\";
//
//     for (const auto &basePath : programFilesPaths)
//     {
//         std::string searchPath = basePath + relativePath;
//
//         try
//         {
//             for (const auto &dirEntry : fs::recursive_directory_iterator(searchPath))
//             {
//                 if (dirEntry.path().filename() == filename)
//                 {
//                     return dirEntry.path().string();
//                 }
//             }
//         }
//         catch (const std::exception &)
//         {
//             // Skip directories that can't be accessed
//         }
//     }
//
//     return filename + " not found!";
// }
//
int main()
{

    // Main
    // spdlog::set_level(spdlog::level::debug);
    // Initialized GLFW
    glfwSetErrorCallback(error_callback);

    // Create a Window
    Window window(1280, 720, "ImSpin");

    // Variables for custom styles

    // Filepath variables
    std::string filepath = "example.pml";
    std::string filecontent;
    bool fileOpened = false, fileSaved = false, fileFormat = false;
    bool window_open        = true;

    double lastTimeSyntax   = glfwGetTime();
    int drawingFrameCounter = 0;
    bool syntaxChecked      = false;

    SpinGui spin;
    while (!window.shouldClose())
    {
        window.pollEvents();
        spin.ProcessUpdates();

        if (!ImGui::IsAnyMouseDown() && drawingFrameCounter > 5)
        {
            glfwWaitEventsTimeout(1);
            drawingFrameCounter = 0;
        }
        drawingFrameCounter++;
        double time = glfwGetTime();
        // window.setClearColor(sin(time), -sin(time), 0, 1);

        // START RENDERING
        if (!window.isFocused)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
            continue;
        }

        window.startDrawing();
        spin.Render();
        window.endDrawing();

        // END RENDERING
    }

    return EXIT_SUCCESS;
}
