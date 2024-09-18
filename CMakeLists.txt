# Minimum required CMake version
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Cyprus VERSION 1.0 LANGUAGES CXX)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find required packages
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add the executable
add_executable(cyprus cyprus.cc)

# Link necessary libraries to the executable
target_link_libraries(cyprus PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# Include necessary directories
target_include_directories(cyprus PRIVATE ${CURL_INCLUDE_DIRS})

# Enable useful warnings and strict compilation flags
target_compile_options(cyprus PRIVATE
    -Wall         # Enable most warning messages
    -Wextra       # Enable extra warning messages
    -Wpedantic    # Enforce strict standard compliance
    -Werror       # Treat warnings as errors
)
