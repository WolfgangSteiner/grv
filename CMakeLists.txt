cmake_minimum_required(VERSION 3.0.0)
project(grv VERSION 0.1.0)
include_directories(include)

if (MSVC)

else()
    set(CMAKE_C_STANDARD_LIBRARIES "${CMAKE_C_STANDARD_LIBRARIES} -lm") 
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Werror -Wno-unused-function")   
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -DGRV_DEBUG_MEMORY")
endif()


add_library(libgrv src/grv.c)

function(add_test test_name)
    add_executable(${test_name} test/${test_name}.c)
    target_link_libraries(${test_name} libgrv)
    add_custom_target(run_${test_name} ALL COMMAND ${test_name} DEPENDS ${test_name})
endfunction()

# execute all tests
file(GLOB TEST_FILES "${CMAKE_SOURCE_DIR}/test/test_*.c")
foreach(FILE_PATH ${TEST_FILES})
    get_filename_component(test_name ${FILE_PATH} NAME_WE)
    add_test(${test_name})
endforeach()
