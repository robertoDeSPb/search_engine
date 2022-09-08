cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)


set(Boost_INCLUDE_DIR "C:\\boost_1_79_0")
set(Boost_LIBRARY_DIR "C:\\boost_1_79_0\\stage\\lib")

find_package(Boost COMPONENTS REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})



include(FetchContent)

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

set(JSON_BuildTests OFF)


enable_testing()
add_subdirectory(Google_tests)

add_executable(search_engine main.cpp invertedIndex.cpp invertedIndex.h searchServer.cpp searchServer.h)


target_link_libraries(search_engine PRIVATE gtest_main nlohmann_json::nlohmann_json ${Boost_LIBRARIES})