#include "window.hpp"
#include <spdlog/spdlog.h>

#include "GLFW/glfw3.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "imgui.h"

void APIENTRY gl_debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity,
                              GLsizei, // length
                              const char *message,
                              const void * // user_param
)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
    {
        // return;
    }

    std::stringstream s;
    s << "---------------" << std::endl;
    s << "Debug message (" << id << "): " << message << std::endl;

    // print log source
    s << "Source: ";
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:
        s << "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        s << "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        s << "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        s << "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        s << "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        s << "Other";
        break;
    }
    s << std::endl;

    // print log type
    s << "Type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        s << "Error";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        s << "Deprecated Behaviour";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        s << "Undefined Behaviour";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        s << "Portability";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        s << "Performance";
        break;
    case GL_DEBUG_TYPE_MARKER:
        s << "Marker";
        break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
        s << "Push Group";
        break;
    case GL_DEBUG_TYPE_POP_GROUP:
        s << "Pop Group";
        break;
    case GL_DEBUG_TYPE_OTHER:
        s << "Other";
        break;
    }
    s << std::endl;

    // print log severity
    s << "Severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:
        s << "high";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        s << "medium";
        break;
    case GL_DEBUG_SEVERITY_LOW:
        s << "low";
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        s << "notification";
        break;
    }
    s << std::endl;

    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        ::printf("%s", s.str().c_str());
    }
}

void focus_callback(GLFWwindow *window, int focus)
{
    Window *w = (Window *)glfwGetWindowUserPointer(window);
    w->isFocused = focus;
}

Window::Window(int32_t width, int32_t height, const char *name)
{
    if (glfwInit() != GLFW_TRUE)
    {
        spdlog::error("GLFW could not be initialized");
        exit(EXIT_FAILURE);
    }

    // Create a Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

    this->w = glfwCreateWindow(width, height, name, NULL, NULL);
    if (!this->w)
    {
        spdlog::error("GLFW the window could not be created");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(this->w);

    // Loading glad
    gladLoadGL((GLADloadfunc)glfwGetProcAddress);

    glfwSwapInterval(1);
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(gl_debug_output, nullptr);
    glfwSetWindowUserPointer(this->w, this);
    glfwSetWindowFocusCallback(this->w, focus_callback);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    // Creation of ImGui
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // IF using Docking Branch
    io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts | ImGuiConfigFlags_DpiEnableScaleViewports;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    // io.Fonts->AddFontFromFileTTF("/usr/share/fonts/TTF/LilexNerdFont-Regular.ttf", 18);

    ImGui_ImplGlfw_InitForOpenGL(this->w, true);
    ImGui_ImplOpenGL3_Init();
}

Window::~Window()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(this->w);
    glfwTerminate();
}

void Window::setKeyCallback(GLFWkeyfun callback)
{
    glfwSetKeyCallback(this->w, callback);
}

void Window::setScrollCallback(GLFWscrollfun callback)
{
    glfwSetScrollCallback(this->w, callback);
}

void Window::setUserPointer(void *pointer)
{
    glfwSetWindowUserPointer(this->w, pointer);
}

void Window::close()
{
    glfwSetWindowShouldClose(this->w, GLFW_TRUE);
}

bool Window::shouldClose()
{
    return glfwWindowShouldClose(this->w);
}

void Window::pollEvents()
{
    glfwPollEvents();
}

void Window::startDrawing()
{
    glfwGetFramebufferSize(this->w, &this->width, &this->height);
    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void Window::endDrawing()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow *backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
    glfwSwapBuffers(this->w);
}

void Window::setClearColor(float r, float g, float b, float a)
{
    glClearColor(r, g, b, a);
}
