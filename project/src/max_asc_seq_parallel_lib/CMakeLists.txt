cmake_minimum_required(VERSION 3.6)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

set(MAX_ASC_SEQ_PARALLEL_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/max_ascending_seq_parallel.c)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage -pthread")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")
set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/project/include)

add_library(max_asc_seq_parallel_lib SHARED ${MAX_ASC_SEQ_PARALLEL_SRCS})
target_include_directories(max_asc_seq_parallel_lib PRIVATE ${INCLUDE_DIRS})
target_link_libraries(max_asc_seq_parallel_lib Threads::Threads)
