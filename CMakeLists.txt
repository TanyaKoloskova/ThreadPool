cmake_minimum_required(VERSION 3.15)
project(ThreadPool)

set(CMAKE_CXX_STANDARD 14)

add_executable(ThreadPool main.cpp ThreadPool.h)
target_link_libraries(ThreadPool pthread)