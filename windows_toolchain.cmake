# toolchain.cmake

set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Find MinGW-w64 installation
find_program(MINGW_C_COMPILER NAMES x86_64-w64-mingw32-gcc REQUIRED)
find_program(MINGW_CXX_COMPILER NAMES x86_64-w64-mingw32-g++ REQUIRED)
find_program(MINGW_RC_COMPILER NAMES x86_64-w64-mingw32-windres REQUIRED)

if(NOT MINGW_C_COMPILER)
    message(FATAL_ERROR "MinGW-w64 gcc not found")
endif()
if(NOT MINGW_CXX_COMPILER)
    message(FATAL_ERROR "MinGW-w64 g++ not found")
endif()
if(NOT MINGW_RC_COMPILER)
    message(FATAL_ERROR "MinGW-w64 windres not found")
endif()

set(CMAKE_C_COMPILER ${MINGW_C_COMPILER})
set(CMAKE_CXX_COMPILER ${MINGW_CXX_COMPILER})
set(CMAKE_RC_COMPILER ${MINGW_RC_COMPILER})

# Set the find root path to MinGW-w64
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32 /usr/local/x86_64-w64-mingw32)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

add_compile_definitions(_WIN32)

set(CMAKE_CXX_STANDARD_LIBRARIES
    "-static-libgcc -static-libstdc++ -lwsock32 -lws2_32 ${CMAKE_CXX_STANDARD_LIBRARIES}"
)
set(CMAKE_EXE_LINKER_FLAGS
    "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic,--whole-archive -lwinpthread -Wl,--no-whole-archive"
)
