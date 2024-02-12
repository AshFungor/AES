cmake_minimum_required(VERSION 3.15)

project(util VERSION 0.0 LANGUAGES CXX)

set(SOURCES numeric.cpp)
set(HEADERS numeric.hpp definitions.hpp)

add_library(util STATIC ${SOURCES} ${HEADERS})

target_include_directories(util PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})