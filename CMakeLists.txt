cmake_minimum_required(VERSION 3.6)
project(tp_hw1)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)

set(MAIN_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/cmd/main.c
        ${CMAKE_CURRENT_SOURCE_DIR}/project/src/task.c)
set(INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/project/include)
set(MAX_ASC_SEQ_DIR ${CMAKE_CURRENT_SOURCE_DIR}/project/src/max_asc_seq_lib/)
set(TEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/project/test/main.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/max_asc_seq_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/project/test/file_test.cpp
        )

add_executable(main ${MAIN_SRCS})
#TODO: убрать паблик
target_include_directories(main PUBLIC ${INCLUDE_DIRS})

add_compile_definitions(PROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/project/")
find_package(GTest REQUIRED)
add_executable(test ${TEST_SRCS})
target_include_directories(test PUBLIC ${GTEST_INCLUDE_DIRS} ${INCLUDE_DIRS})
target_link_libraries(test ${GTEST_LIBRARIES})

if (parallel)
    add_library(max_asc_seq_parallel ${MAX_ASC_SEQ_PARALLEL_SRCS})
    target_link_libraries(main max_asc_seq_parallel)
else ()
    #TODO: добавить доп аргументы в адд саб директори
    add_subdirectory(${MAX_ASC_SEQ_DIR})
    target_link_libraries(main PUBLIC max_asc_seq)
    target_link_libraries(test max_asc_seq)
endif (parallel)






