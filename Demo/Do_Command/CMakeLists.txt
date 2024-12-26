project(Do_Command)

add_executable(do_command do_command.cpp)
target_include_directories(do_command PRIVATE ../../Utils/Do_Command)
target_link_libraries(do_command PRIVATE library_utils)