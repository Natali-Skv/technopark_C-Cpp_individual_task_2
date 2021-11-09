cmake_minimum_required(VERSION 3.6)
project(tp_hw2)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall  -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread -Wno-unknown-pragmas -Wno-sign-compare  -Wwrite-strings -Wno-unused")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -Wno-unknown-pragmas -Wno-sign-compare -Wwrite-strings -Wno-unused ")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)

set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/project/include)
set(MAX_ASC_SEQ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/project/src/max_asc_seq_lib/)
set(MAX_ASC_SEQ_PARALLEL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/project/src/max_asc_seq_parallel_lib/)
set(MAIN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/cmd/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/task.c
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/file.c)
set(TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/max_asc_seq_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/file_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/file.c
        )
set(STRESS_TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/stress_test.cpp)
option(USE_PARALLEL "Enable using parallel library" NO)

add_executable(main ${MAIN_SRCS})
target_include_directories(main PRIVATE ${INCLUDE_DIRS})

add_compile_definitions(PROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/project/")

find_package(GTest REQUIRED)
add_executable(test_sinc ${TEST_SRCS})
target_include_directories(test_sinc PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_sinc max_asc_seq)
target_link_libraries(test_sinc ${GTEST_LIBRARIES})
target_compile_definitions(test_sinc PRIVATE RESULT_FILE="${CMAKE_CURRENT_SOURCE_DIR}/project/test/fin_fout/sinc_result")

add_executable(test_asinc ${TEST_SRCS})
target_include_directories(test_asinc PRIVATE ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test_asinc max_asc_seq_parallel)
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
    target_link_libraries(main PRIVATE max_asc_seq_parallel )
    message("SH")
else ()
    message("ST")
    target_link_libraries(main PRIVATE max_asc_seq)
endif ()

# -DUSE_PARALLEL=1


