include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest
        GIT_TAG        release-1.12.1
)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED ./invertedindex.cpp ../invertedindex.h ../searchserver.cpp ../searchserver.h)
target_link_libraries(GTest::GTest INTERFACE gtest_main)



add_executable(ss_test
        invertedIndexTest.cpp
        ../invertedindex.cpp
        ../invertedindex.h
        searchServerTests.cpp
        ../searchserver.cpp
        ../searchserver.h)

target_link_libraries(ss_test
        PRIVATE
        GTest::GTest)