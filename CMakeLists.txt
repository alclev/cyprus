cmake_minimum_required(VERSION 3.10)
project(cyprus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(cyprus cyprus.cc)
target_link_libraries(cyprus PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)
target_include_directories(cyprus PRIVATE ${CURL_INCLUDE_DIRS})