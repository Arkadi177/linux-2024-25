project(Threads)

add_executable(l_thread LinuxThread.cpp)
add_executable(l_mutex LinuxMutex.cpp)
add_executable(l_mutex_guard LinuxMutexGuard.cpp)
add_executable(main main.cpp)

target_include_directories(l_thread  PRIVATE ../../Utils/Threads)
target_include_directories(l_mutex  PRIVATE ../../Utils/Threads)
target_include_directories(l_mutex_guard  PRIVATE ../../Utils/Threads)
