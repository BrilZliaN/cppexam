cmake_minimum_required(VERSION 3.9)
project(optional)

set(CMAKE_CXX_FLAGS "-pthread -Wall -std=c++14 -pedantic ${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "-D_GLIBCXX_DEBUG ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_COVERAGE "-g --coverage")
set(LINK_FLAGS "-pthread ${LINK_FLAGS}")
#[[set(CMAKE_CXX_STANDARD 11)]]

include_directories(.)
add_subdirectory(gtest)

add_library(counted counted.h counted.cpp tests.inl)

add_executable(optional main.cpp optional.h)
target_link_libraries(optional counted gtest)