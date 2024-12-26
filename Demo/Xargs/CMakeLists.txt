project(Do_Command)

add_executable(xargs do_command_xargs.cpp)
target_include_directories(xargs PRIVATE ../../Utils/Do_Command)