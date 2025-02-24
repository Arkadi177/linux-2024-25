# Specify the minimum version of CMake required
cmake_minimum_required(VERSION 3.10)

# Project name
project(Utils)

# Add source files (your .cpp files)
set(SOURCES
        Allocator/my_allocator.cpp
        Directory_watcher/dr_watcher.cpp
        Do_Command/do_command.cpp
        Iterator/ArgumentParser.cpp
        Iterator/CircularIterator.cpp
        Iterator/Directory.cpp
        Iterator/FilterIntegerIterator.cpp
        Iterator/ReverseArrayIterator.cpp
        Ser_Des/ser_des.cpp
)

add_library(utils_static STATIC ${SOURCES})

#add_library(utils_shared SHARED ${SOURCES})

target_include_directories(utils_static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
#target_include_directories(utils_shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

