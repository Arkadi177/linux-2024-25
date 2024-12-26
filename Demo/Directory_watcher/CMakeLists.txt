cmake_minimum_required(VERSION 3.10)
project(watcher_demo)

set(CMAKE_CXX_STANDARD 17)

# Add the main executable
add_executable(watcher_demo watcher_demo.cpp)

# Include directories
target_include_directories(watcher_demo PRIVATE
        ../../Utils/Directory_watcher
        ../../Utils/Iterator
)

target_link_directories(watcher_demo PRIVATE library_utils)

# Print useful debugging information
message(INFO "CMake command: ${CMAKE_COMMAND}")
message(INFO "Current list directory: ${CMAKE_CURRENT_LIST_DIR}")

# Post-build step to copy the output file (if intended to copy the built executable)
add_custom_command(TARGET watcher_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:watcher_demo>
        ${CMAKE_CURRENT_LIST_DIR}/watcher_demo
)
