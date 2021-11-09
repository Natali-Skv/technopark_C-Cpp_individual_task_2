// Created by ns on 09.11.2021.
#include <iostream>
#include <zconf.h>

/*
extern "C" {
#include <max_ascending_seq.h>
#include <file.h>
}

#include <gtest/gtest.h>
#include <cstring>
#include <fstream>

class Fixture : public ::testing::Test {
protected:
    void SetUp(const char *finPath, size_t expLen) {
        int array[5] = {1,2,3,4,1};
        size_t size = 5u;

        EXPECT_EQ(get_max_asc_seq_len(array, size), get_max_asc_seq_len(array, size));

        //free(array);
        close_file(fin);
    }
};*/



#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char*argv[]) {
    int pid, status;
    pid = fork();

    if (pid == -1) {
        std::cout << "fork() faild";
        return 0;
    }

    if (pid == 0) {
        // пробросить через cmake
        execl(TEST_SINC_PATH, TEST_SINC_PATH, NULL);
        exit(EXIT_SUCCESS);
    }
    execl(TEST_ASINC_PATH, TEST_ASINC_PATH, NULL);
    wait(&status);

}