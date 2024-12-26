project(Allocator)

add_executable(allocator gpt_test_allocator.cpp)
target_include_directories(allocator PRIVATE ../../Utils/Allocator)
target_link_libraries(allocator PRIVATE utils_static)
#target_link_directories(allocator PRIVATE utils_shared)
