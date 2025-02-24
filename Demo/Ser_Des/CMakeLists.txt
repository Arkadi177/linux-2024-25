project(Ser_Des)

add_executable(Serialazation_tests main.cpp test_case_1.cpp test_case_2.cpp test_case_3.cpp test_case_4_not_copiable.cpp)

target_include_directories(Serialazation_tests PRIVATE ../../Utils/Ser_Des)
target_link_libraries(Serialazation_tests PRIVATE utils_static)