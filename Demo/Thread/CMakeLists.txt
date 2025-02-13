project(Blocking_queue)

add_executable(block_queue BlockingQueue.cpp)
target_include_directories(block_queue PRIVATE ../../Utils/Thread)
