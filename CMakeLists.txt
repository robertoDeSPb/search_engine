cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)

set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_subdirectory(Google_tests)
add_executable(search_engine main.cpp Google_tests/invertedIndexTest.cpp invertedIndex.cpp invertedIndex.h searchServer.cpp searchServer.h Google_tests/searchServerTests.cpp)


target_link_libraries(search_engine PRIVATE gtest nlohmann_json::nlohmann_json)

