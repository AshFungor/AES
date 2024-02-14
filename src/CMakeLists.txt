cmake_minimum_required(VERSION 3.15)

project(AES VERSION 0.0 LANGUAGES CXX)

add_subdirectory(util)
add_subdirectory(algorithm)
add_subdirectory(tests)

add_executable(AES main.cpp)

target_link_libraries(AES PRIVATE util)
target_link_libraries(AES PRIVATE algorithm)
