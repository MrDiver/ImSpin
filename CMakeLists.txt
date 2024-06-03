cmake_minimum_required(VERSION 3.24)

# CONFIG FOR CPP 20
option(INSTALL_DEPENDENCIES "Installs dependencies for ImSpin (spin, swarm)" ON)

# set(CMAKE_CONFIGURATION_TYPES Debug Release)

project("imspin" C CXX)

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
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.3)

if(NOT boost_POPULATED AND NOT EXISTS
                           ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/boost)
  FetchContent_Populate(
    boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.85.0
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/boost)
endif()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/boost)

FetchContent_Declare(
  imedit GIT_REPOSITORY https://github.com/BalazsJako/ImGuiColorTextEdit.git)

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

FetchContent_MakeAvailable(
  imgui
  spdlog
  glfw
  glm
  nfd
  imedit
  json)

if(INSTALL_DEPENDENCIES)
  FetchContent_Declare(
    spin
    GIT_REPOSITORY https://gitlab.com/MrDiver/spin.git
    GIT_TAG 7e4074411be38c5902e7d021625172421822e066)
  FetchContent_Declare(
    swarm
    GIT_REPOSITORY https://gitlab.com/MrDiver/swarm.git
    GIT_TAG 8bfa0251edb5f74195fd8fc5a6360e2dab641224)
  FetchContent_MakeAvailable(spin swarm)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# CONFIG EXECUTABLE
set(SOURCE_FILES "src/editor.cpp" "src/ezgl.cpp" "src/main.cpp"
                 "src/window.cpp" "src/config.cpp" "src/spinwrap.cpp")
set(HEADER_FILES
    "include/colors.hpp"
    "include/editor.hpp"
    "include/ezgl.hpp"
    "include/fonts.hpp"
    "include/window.hpp"
    "include/config.hpp"
    "include/spinwrap.hpp")

if(NOT WIN32 OR MINGW)
  set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/dist)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES}
                               ${imedit_SOURCE_DIR}/TextEditor.cpp)
target_include_directories(${PROJECT_NAME}
                           PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

target_include_directories(${PROJECT_NAME} PUBLIC "${imedit_SOURCE_DIR}")

# LINK GLFW
set(GLFW_LIBRARY_TYPE "SHARED")
set(GLFW_STANDALONE OFF)
set(GLFW_BUILD_EXAMPLES ${GLFW_STANDALONE})
set(GLFW_BUILD_TESTS ${GLFW_STANDALONE})
set(GLFW_BUILD_DOCS OFF)
set(GLFW_INSTALL OFF)
include_directories(${glfw_SOURCE_DIR}/include)

# ADD glad
if(WIN32)
  find_package(OpenGL REQUIRED)
  if(OPENGL_FOUND)
    message("opengl found")
    message("include dir: ${OPENGL_INCLUDE_DIR}")
    message("link libraries: ${OPENGL_gl_LIBRARY}")
  else(OPENGL_FOUND)
    message("opengl not found")
  endif()
  target_link_libraries(${PROJECT_NAME} PUBLIC OpenGL::GL)
else()
  target_link_libraries(${PROJECT_NAME} PUBLIC GL)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC glad_gl)

# ADD glm
target_link_libraries(${PROJECT_NAME} PUBLIC glm)

# Configure IMGUI
set(IMGUI_BACKEND "glfw")
set(IMGUI_API "opengl3")
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
add_executable(binary_to_comrpessed_c
               ${imgui_SOURCE_DIR}/misc/fonts/binary_to_compressed_c.cpp)

# LINK LIBRARIES
target_link_libraries(${PROJECT_NAME} PUBLIC imgui spdlog nfd
                                             nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} PRIVATE Boost::process Boost::xpressive glfw)
target_include_directories(${PROJECT_NAME} PUBLIC "external/imgui")

if(MINGW)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD DEPENDS spin swarm
    COMMAND ${CMAKE_COMMAND} -E copy "${spin_BINARY_DIR}/src/spin.exe"
            ${CMAKE_CURRENT_SOURCE_DIR}/dist/extra/spin.exe
    COMMAND ${CMAKE_COMMAND} -E copy "${swarm_BINARY_DIR}/src/swarm.exe"
            ${CMAKE_CURRENT_SOURCE_DIR}/dist/extra/swarm.exe)
endif()
if(WIN32 AND NOT MINGW)
  target_compile_definitions(${PROJECT_NAME} PRIVATE PC)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD DEPENDS spin swarm
    COMMAND
      ${CMAKE_COMMAND} -E copy
      "${spin_BINARY_DIR}/src/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>/spin.exe"
      ${CMAKE_CURRENT_SOURCE_DIR}/extra/spin.exe
    COMMAND
      ${CMAKE_COMMAND} -E copy
      "${swarm_BINARY_DIR}/src/$<$<CONFIG:Debug>:Debug>$<$<CONFIG:Release>:Release>$<$<CONFIG:RelWithDebInfo>:Release>/swarm.exe"
      ${CMAKE_CURRENT_SOURCE_DIR}/extra/swarm.exe)
  add_compile_definitions(_WIN32_WINNT=0x0601)
  
    #target_compile_options(${PROJECT_NAME} PRIVATE /fsanitize=address /O)
    #target_link_options(${PROJECT_NAME} PRIVATE /fsanitize=address)
endif()
if(LINUX)
  add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD DEPENDS spin swarm
    COMMAND ${CMAKE_COMMAND} -E copy "${spin_BINARY_DIR}/src/spin"
            ${CMAKE_CURRENT_SOURCE_DIR}/dist/extra/spin
    COMMAND ${CMAKE_COMMAND} -E copy "${swarm_BINARY_DIR}/src/swarm"
            ${CMAKE_CURRENT_SOURCE_DIR}/dist/extra/swarm)
endif()

# if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
set(CMAKE_INSTALL_PREFIX
    "PACKAGE"
    CACHE PATH "..." FORCE)
# endif()

if(LINUX)
  if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O2)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE -fsanitize=address -O)
    target_link_options(${PROJECT_NAME} PRIVATE -fsanitize=address)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _FORTIFY_SOURCE)
  endif()
endif()

if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/dist)
  file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/dist)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION "bin")
install(TARGETS spin DESTINATION "bin/extra")
install(TARGETS swarm DESTINATION "bin/extra")
