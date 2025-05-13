project(Spionen_handler)

add_executable(spionen_demo spionen.cpp)
add_executable(comissioner_demo commissioner.cpp)
add_executable(all_run all_run.cpp)

target_include_directories(all_run PRIVATE ../../Utils/Do_Command)
target_link_libraries(all_run PRIVATE utils_static)