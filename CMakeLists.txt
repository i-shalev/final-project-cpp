cmake_minimum_required(VERSION 3.15)
project(final_project_cpp)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(final_project_cpp
        src/main.cpp
        src/main.h)
