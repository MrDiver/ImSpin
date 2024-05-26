cmake_minimum_required(VERSION 3.20)

# CREATE EXECUTABLE

project("imspin" C CXX)

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
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm.git
  GIT_TAG 1.0.1)

set(NFD_PORTAL ON)
FetchContent_Declare(
  nfd
  GIT_REPOSITORY https://github.com/btzy/nativefiledialog-extended.git
  GIT_TAG v1.1.1)

# FetchContent_Declare( imfile GIT_REPOSITORY
# https://github.com/aiekick/ImGuiFileDialog.git GIT_TAG v0.6.7)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad.git
  GIT_TAG v2.0.6)

if(NOT glad_POPULATED)
    FetchContent_Populate(glad)
    add_subdirectory("${glad_SOURCE_DIR}/cmake" glad_cmake)
    glad_add_library(glad_gl REPRODUCIBLE API gl:compatibility=4.3)
endif()

FetchContent_MakeAvailable(imgui spdlog glfw glm nfd)

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
set(CMAKE_C_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CONFIG EXECUTABLE
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")
file(GLOB_RECURSE HEADER_FILES "include/*.hpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})
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
target_link_libraries(${PROJECT_NAME} PUBLIC GL)
target_link_libraries(${PROJECT_NAME} PUBLIC glad_gl)

# ADD glm
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

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
# target_link_libraries(ImGuiFileDialog PUBLIC imgui)
target_link_libraries(${PROJECT_NAME} PUBLIC imgui glfw spdlog nfd)
# target_sources(${PROJECT_NAME} PRIVATE ${IMGUI_SOURCES} ${IMGUI_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE "external/imgui")
