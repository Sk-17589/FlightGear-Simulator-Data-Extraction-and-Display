cmake_minimum_required(VERSION 3.10)
project(FlightGearDataExtractorMFC)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED)

# Include directories
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Add executable
add_executable(FlightGearDataExtractorMFC main.cpp)

# Link libraries
target_link_libraries(FlightGearDataExtractorMFC PRIVATE CURL::libcurl nlohmann_json::nlohmann_json)



