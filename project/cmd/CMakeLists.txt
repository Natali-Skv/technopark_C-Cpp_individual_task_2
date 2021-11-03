cmake_minimum_required(VERSION 3.6)
project(tp_hw1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDART_REQURED ON)

set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs")
set(CMAKE_C_FLAGS "-g -O0 -Wall -Werror -Wextra -Wno-unused-variable -Wpedantic -fprofile-arcs")


set(MAIN_SRCS main.c)

add_executable(main ${MAIN_SRCS})

target_link_libraries(main PUBLIC file data_storage length_calculator input_processing)

if(parallel)
    target_link_libraries(main PUBLIC collector_parallel)
else()
    target_link_libraries(main PUBLIC collector)
endif(parallel)

install(TARGETS main DESTINATION bin)