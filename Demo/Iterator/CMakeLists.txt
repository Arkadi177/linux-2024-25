cmake_minimum_required(VERSION 3.16)
project(Iterator)

add_executable(circular_iterator CircularIntIterator.cpp)
target_include_directories(circular_iterator PRIVATE ../../Utils/)

add_executable(filter_iterator FilterIntegerIterator.cpp)
target_include_directories(filter_iterator PRIVATE ../../Utils/)

add_executable(reverse_iterator ReverseArrayIterator.cpp)
target_include_directories(reverse_iterator PRIVATE ../../Utils/)

add_executable(ArgumentParserDemo ArgumentParser.cpp)
target_include_directories(ArgumentParserDemo PRIVATE ../../Utils/)