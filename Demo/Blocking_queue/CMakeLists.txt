project(Blocking_queue)

add_executable(block_queue blocking_queue.cpp)
target_include_directories(block_queue PRIVATE ../../Utils/Blocking_queue)
target_link_libraries(block_queue PRIVATE utils_static)