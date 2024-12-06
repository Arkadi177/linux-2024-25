project(Directory_watcher)

add_executable(watcher_test watcher_tests.cpp)

target_include_directories(watcher_test PRIVATE ../../Utils/Directory_watcher)
target_include_directories(watcher_test PRIVATE ../../Utils/Iterator)

