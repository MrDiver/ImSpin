cmake_minimum_required(VERSION 3.20)

# CREATE EXECUTABLE

project("imspin")

# CONFIG FOR CPP 20
option(INSTALL_DEPENDENCIES "Installs dependencies for ImSpin (spin, swarm)" ON)

include(FetchContent)

FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.90.6-docking)

FetchContent_Declare(
  spdlog
  GIT_REPOSITORY https://github.com/gabime/spdlog.git
  GIT_TAG v1.14.1)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.6)

# set(GLAD_PROFILE "core" CACHE STRING "OpenGL profile")
set(GLAD_GENERATOR
    "c"
    CACHE STRING "Language to generate the binding for")

FetchContent_MakeAvailable(imgui spdlog glfw glad)

if(INSTALL_DEPENDENCIES)
  FetchContent_Declare(
    spin
    GIT_REPOSITORY https://gitlab.com/ktuff/spin.git
    GIT_TAG 46b53e255bee03fdb5b241817408f389cffc6016)
  FetchContent_Declare(
    swarm
    GIT_REPOSITORY https://gitlab.com/ktuff/swarm.git
    GIT_TAG 95fb3442fb4ed67e7391e41c675c8aa19f3ad232)
  FetchContent_MakeAvailable(spin swarm)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# CONFIG EXECUTABLE
set(SOURCE_FILES "src/main.cpp" "src/window.cpp" "src/ezgl.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(${PROJECT_NAME}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")

# LINK GLFW
set(GLFW_LIBRARY_TYPE "SHARED")
set(GLFW_STANDALONE OFF)
set(GLFW_BUILD_EXAMPLES ${GLFW_STANDALONE})
set(GLFW_BUILD_TESTS ${GLFW_STANDALONE})
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)

# ADD glad
target_link_libraries(${PROJECT_NAME} gl)
target_link_libraries(${PROJECT_NAME} glad)

# LINK IMGUI
set(IMGUI_BACKEND "glfw")
set(IMGUI_API "opengl3")
message(${imgui_SOURCE_DIR})
file(GLOB IMGUI_SOURCES "${imgui_SOURCE_DIR}/*.cpp")
file(GLOB IMGUI_HEADERS "${imgui_SOURCE_DIR}/*.h")

add_library(
  imgui STATIC
  ${IMGUI_SOURCES}
  ${IMGUI_HEADERS}
  "${imgui_SOURCE_DIR}/backends/imgui_impl_${IMGUI_BACKEND}.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_${IMGUI_BACKEND}.h"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_${IMGUI_API}.cpp"
  "${imgui_SOURCE_DIR}/backends/imgui_impl_${IMGUI_API}.h")

target_include_directories(imgui PUBLIC "${imgui_SOURCE_DIR}")
target_link_libraries(${PROJECT_NAME} imgui glfw spdlog)
# target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "external/imgui")
