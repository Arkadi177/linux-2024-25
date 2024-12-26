project(Do_Command)

add_executable(do_command do_command.cpp)
target_include_directories(do_command PRIVATE ../../Utils/Do_Command)

add_executable(do_command do_command_xargs.cpp)