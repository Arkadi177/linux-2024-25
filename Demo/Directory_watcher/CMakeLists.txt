project(Directory_watcher)

add_executable(watcher_test1 dr_watcher_test_1.cpp)
add_executable(watcher_test2 dr_watcher_test_2.cpp)

target_include_directories(watcher_test1 PRIVATE ../../Utils/Directory_watcher)
target_include_directories(watcher_test2 PRIVATE ../../Utils/Directory_watcher)
