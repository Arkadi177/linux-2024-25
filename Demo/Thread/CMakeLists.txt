project(Blocking_queue)

add_executable(block_queue BlockingQueue.cpp)
add_executable(thread_pool ThreadPool.cpp)

target_include_directories(block_queue PRIVATE ../../Utils/Thread)
target_include_directories(thread_pool PRIVATE ../../Utils/Thread)
