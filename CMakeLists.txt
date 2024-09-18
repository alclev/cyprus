cmake_minimum_required(VERSION 3.10)
project(Cyprus VERSION 1.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find required packages
find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

# Add the executable
add_executable(cyprus cyprus.cc)

# Include directories
target_include_directories(cyprus PRIVATE ${CURL_INCLUDE_DIRS})

# Link libraries
target_link_libraries(cyprus PRIVATE ${CURL_LIBRARIES} nlohmann_json::nlohmann_json)

# Set compile options
target_compile_options(cyprus PRIVATE -Wall -Wextra)

# Install the executable
install(TARGETS cyprus DESTINATION bin)