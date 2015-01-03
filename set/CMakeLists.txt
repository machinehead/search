cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

project(set)

add_executable(test_set test_set.cpp)

target_link_libraries(test_set nvwa)