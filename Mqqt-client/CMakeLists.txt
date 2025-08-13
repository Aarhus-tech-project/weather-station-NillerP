cmake_minimum_required(VERSION 3.16)

# Project
project(HelloWin LANGUAGES CXX)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Helpful for tooling (VSCode, clangd, etc.)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Collect sources from /src
file(GLOB_RECURSE APP_SOURCES CONFIGURE_DEPENDS
     "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Executable
add_executable(${PROJECT_NAME} ${APP_SOURCES})

# Public headers live in /include
target_include_directories(${PROJECT_NAME}
    PRIVATE
        "${CMAKE_SOURCE_DIR}/include"
)

# Add these lines before target_link_libraries
include_directories("C:/msys64/mingw64/include")
link_directories("C:/msys64/mingw64/lib")
target_link_libraries(${PROJECT_NAME} PRIVATE pqxx pq)
target_link_libraries(${PROJECT_NAME} PRIVATE pqxx)
target_link_libraries(${PROJECT_NAME} PRIVATE mosquitto)

# Nice warnings on MinGW/Clang/GCC
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# If you want the binary in ./bin (optional)
# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")