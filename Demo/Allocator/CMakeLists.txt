project(Allocator)

add_executable(allocator my_allocator.cpp)
target_include_directories(allocator PRIVATE ../../Utils/Allocator)