cmake_minimum_required(VERSION 3.15)

set(TESTS util-test.cpp aes-test.cpp)

add_executable(test ${TESTS})

target_link_libraries(test PRIVATE GTest::gtest_main)
target_link_libraries(test PRIVATE util)
target_link_libraries(test PRIVATE algorithm)
