cmake_minimum_required(VERSION 3.10)
project(watcher_demo)

set(CMAKE_CXX_STANDARD 17)

add_executable(watcher_demo watcher_demo.cpp)

target_include_directories(watcher_demo PRIVATE ../../Utils/Directory_watcher)
target_include_directories(watcher_demo PRIVATE ../../Utils/Iterator)

