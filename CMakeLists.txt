cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories(list nvwa tree set measure)

add_subdirectory(list)
add_subdirectory(nvwa)
add_subdirectory(tree)
add_subdirectory(set)
add_subdirectory(graph)

add_executable(main a.cpp)

#target_link_libraries(main list)
