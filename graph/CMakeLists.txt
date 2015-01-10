cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

project(graph)

#set(SOURCE_LIB .cpp)

#add_library(graph STATIC ${SOURCE_LIB})

add_executable(test_graph test_graph.cpp)

target_link_libraries(test_graph nvwa)