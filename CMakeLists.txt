cmake_minimum_required(VERSION 3.6)
project(tp_hw2)
# option "-DUSE_PARALLEL=1" for use shared asinc library in main

set(CMAKE_CXX_FLAGS "-g -O0 -Wall  -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread -Wno-unknown-pragmas -Wno-sign-compare  -Wwrite-strings -Wno-unused")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -Wno-unknown-pragmas -Wno-sign-compare -Wwrite-strings -Wno-unused ")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/project/include)
set(MAX_ASC_SEQ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/project/src/max_asc_seq_lib/)
set(MAX_ASC_SEQ_PARALLEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/project/src/max_asc_seq_parallel_lib/)
set(MAIN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/cmd/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/task.c)
set(TEST_MAX_ASC_SEQ_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/max_asc_seq_test.cpp
        )
set(TEST_FILE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/file_test.cpp
        )
set(FILE_LIB_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/src/file.c)
set(STRESS_TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/stress_test.cpp)
option(USE_PARALLEL "Enable using parallel library" NO)
add_compile_definitions(PROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/project/")

add_library(file_lib ${FILE_LIB_SRCS})
target_include_directories(file_lib PRIVATE ${INCLUDE_DIRS})

add_executable(main ${MAIN_SRCS})
target_link_libraries(main PRIVATE file_lib)
target_include_directories(main PRIVATE ${INCLUDE_DIRS})

find_package(GTest REQUIRED)

add_executable(test_file ${TEST_FILE_SRCS})
target_include_directories(test_file PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_file file_lib)
target_link_libraries(test_file ${GTEST_LIBRARIES})

add_executable(test_sinc ${TEST_MAX_ASC_SEQ_SRCS})
target_include_directories(test_sinc PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_sinc max_asc_seq_lib file_lib)
target_link_libraries(test_sinc ${GTEST_LIBRARIES})
target_compile_definitions(test_sinc PRIVATE RESULT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/project/test/fin_fout/sinc_result")

add_executable(test_asinc ${TEST_MAX_ASC_SEQ_SRCS})
target_include_directories(test_asinc PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_asinc max_asc_seq_parallel_lib file_lib)
target_link_libraries(test_asinc ${GTEST_LIBRARIES})
target_compile_definitions(test_asinc PRIVATE RESULT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/project/test/fin_fout/asinc_result")

add_executable(stress_test ${STRESS_TEST_SRCS})
target_compile_definitions(stress_test PRIVATE TEST_SINC_PATH="${CMAKE_BINARY_DIR}/test_sinc")
target_compile_definitions(stress_test PRIVATE TEST_ASINC_PATH="${CMAKE_BINARY_DIR}/test_asinc")
target_compile_definitions(stress_test PRIVATE SINC_RESULT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/project/test/fin_fout/asinc_result")
target_compile_definitions(stress_test PRIVATE ASINC_RESULT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/project/test/fin_fout/sinc_result")

add_subdirectory(${MAX_ASC_SEQ_PARALLEL_DIR})
add_subdirectory(${MAX_ASC_SEQ_DIR})

if (USE_PARALLEL)
    target_link_libraries(main PRIVATE max_asc_seq_parallel_lib )
    message("SH")
else ()
    message("ST")
    target_link_libraries(main PRIVATE max_asc_seq_lib)
endif ()
