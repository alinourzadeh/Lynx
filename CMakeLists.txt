cmake_minimum_required(VERSION 3.29)
project(Lynx)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(Lynx server.cpp
        server.cpp)
