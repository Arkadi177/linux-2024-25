cmake_minimum_required(VERSION 3.16)
project(Iterator)

add_executable(circular_iterator CircularIntIterator.cpp)
target_include_directories(circular_iterator PRIVATE ../../Utils/)
target_link_libraries(circular_iterator PRIVATE library_utils)

add_executable(filter_iterator FilterIntegerIterator.cpp)
target_include_directories(filter_iterator PRIVATE ../../Utils/)
target_link_libraries(filter_iterator PRIVATE library_utils)

add_executable(reverse_iterator ReverseArrayIterator.cpp)
target_include_directories(reverse_iterator PRIVATE ../../Utils/)
target_link_libraries(reverse_iterator PRIVATE library_utils)

add_executable(directory_iterator Directory.cpp)
target_include_directories(directory_iterator PRIVATE ../../Utils/Iterator)
target_link_libraries(directory_iterator PRIVATE library_utils)

add_executable(ArgumentParserDemo ArgumentParser.cpp)
target_include_directories(ArgumentParserDemo PRIVATE ../../Utils/)
target_link_libraries(ArgumentParserDemo PRIVATE library_utils)