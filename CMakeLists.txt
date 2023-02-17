cmake_minimum_required(VERSION 3.0.0)
project(grv VERSION 0.1.0)

add_executable(grv_test_str test/test_grv_str.c src/grv_str.c)
include_directories(include)
