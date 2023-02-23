cmake_minimum_required(VERSION 3.0.0)
project(grv VERSION 0.1.0)
include_directories(include)

add_executable(test_grv_str test/test_grv_str.c src/grv.c)

# build executable for testing grv_strarr
add_executable(test_grv_strarr test/test_grv_strarr.c src/grv.c)

# run all executables
add_custom_target(run_tests ALL
    COMMAND test_grv_str
    COMMAND test_grv_strarr
    DEPENDS test_grv_str test_grv_strarr
    COMMENT "Running tests"
)