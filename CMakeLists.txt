# Minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Cyprus VERSION 1.0)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find libcurl package
find_package(CURL REQUIRED)

# Find nlohmann_json package (if installed via package manager)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add the executable
add_executable(cyprus cyprus.cc)

# Link libraries to the executable
target_link_libraries(cyprus PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)

# Add include directories (if necessary)
target_include_directories(cyprus PRIVATE ${CURL_INCLUDE_DIRS})

# Set compilation flags (optional)
target_compile_options(cyprus PRIVATE -Wall -Wextra -pedantic)
