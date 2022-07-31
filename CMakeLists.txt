cmake_minimum_required(VERSION 3.21)
project(search_engine)

set(CMAKE_CXX_STANDARD 14)


set(Boost_INCLUDE_DIR "C:\\boost_1_79_0")
set(Boost_LIBRARY_DIR "C:\\boost_1_79_0\\stage\\lib")

find_package(Boost COMPONENTS REQUIRED)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})


set(JSON_BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)

add_subdirectory(Google_tests)
add_executable(search_engine main.cpp Google_tests/invertedIndexTest.cpp invertedIndex.cpp invertedIndex.h searchServer.cpp searchServer.h Google_tests/searchServerTests.cpp)


target_link_libraries(search_engine PRIVATE gtest nlohmann_json::nlohmann_json ${Boost_LIBRARIES})

