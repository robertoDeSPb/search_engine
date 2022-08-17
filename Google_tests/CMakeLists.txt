# 'Google_test' is the subproject name
project(Google_tests)


include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(Google_Tests_run
        invertedIndexTest.cpp
        ../invertedindex.cpp
        ../invertedindex.h
        searchServerTests.cpp
        ../searchserver.cpp
        ../searchserver.h)
target_link_libraries(Google_Tests_run googletest gtest_main)