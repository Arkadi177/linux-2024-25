project(Allocator)

add_executable(allocator gpt_test_allocator.cpp)
target_include_directories(allocator PRIVATE ../../Utils/Allocator)