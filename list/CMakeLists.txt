cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

project(list)

#set(SOURCE_LIB single_list.cpp)

#add_library(list STATIC ${SOURCE_LIB})

add_executable(test_single test_single.cpp)

target_link_libraries(test_single nvwa)