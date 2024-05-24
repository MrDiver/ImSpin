cmake_minimum_required(VERSION 3.20)

project("GLAD")
add_library(${PROJECT_NAME} STATIC "${CMAKE_CURRENT_SOURCE_DIR}/gl.h" "${CMAKE_CURRENT_SOURCE_DIR}/gl.c")
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
