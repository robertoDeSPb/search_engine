# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

#add_subdirectory("D:/Programm/SkillBOXProjects/lessons.git/final_project/lib")
include_directories(../nlohmann_json/include)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_run
        invertedIndexTest.cpp
        ../invertedindex.cpp
        ../invertedindex.h
        searchServerTests.cpp
        ../searchserver.cpp
        ../searchserver.h)
target_link_libraries(Google_Tests_run gtest gtest_main)