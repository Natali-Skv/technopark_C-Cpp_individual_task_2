// Copyright 2021 nat-s.skv@mail.ru
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>

const size_t bufSize = 100u;

bool AreFilesEq(FILE *f1, FILE *f2) {
    if (!f1 || !f2) {
        return false;
    }
    if (ferror(f1) || ferror(f2)) {
        return false;
    }
    fseek(f1, 0, SEEK_SET);
    fseek(f2, 0, SEEK_SET);
    char buf1[bufSize];
    char buf2[bufSize];
    while (!feof(f1) && !feof(f2)) {
        if (!fgets(buf1, bufSize, f1)) {
            buf1[0] = 0;
        }
        if (!fgets(buf2, bufSize, f2)) {
            buf2[0] = 0;
        }
        if (std::strcmp(buf1, buf2) != 0) {
            return false;
        }
    }
    if (!feof(f1) || !feof(f2)) {
        return false;
    }
    return true;
}

int ExecuteInOtherProc(const char *binName) {
    if (!binName) {
        return 0;
    }
    if (!binName[0]) {
        return 0;
    }
    int pid = fork();

    if (pid == 0) {
        execl(binName, binName, NULL);
        exit(EXIT_SUCCESS);
    }
    return pid;
}

int main() {
    if (ExecuteInOtherProc(TEST_ASINC_PATH) == -1) {
        std::perror("fork() faild");
        std::cout << "stress test has been failed";
        return 0;
    }

    if (ExecuteInOtherProc(TEST_SINC_PATH) == -1) {
        std::perror("fork() faild");
        std::cout << "stress test has been failed";
        return 0;
    }

    int status = 0u;
    wait(&status);
    if (status != EXIT_SUCCESS) {
        std::perror("fork() faild");
        std::cout << "stress test has been failed";
        return 0;
    }

    wait(&status);
    if (status != EXIT_SUCCESS) {
        std::perror("fork() faild");
        std::cout << "stress test has been failed";
        return 0;
    }

    FILE *asinc_result = fopen(ASINC_RESULT_FILE, "r");
    if (!asinc_result) {
        std::perror("open \'asinc_result\' file failed");
        std::cout << "stress test has been failed";
        return 0;
    }
    FILE *sinc_result = fopen(SINC_RESULT_FILE, "r");
    if (!sinc_result) {
        std::perror("open \'sinc_result\' file failed");
        std::cout << "stress test has been failed";
        fclose(asinc_result);
        return 0;
    }

    if (ferror(asinc_result) || ferror(sinc_result)) {
        std::perror(R"(there is some error in 'sinc_result' file or 'asinc_result' file)");
        std::cout << "stress test has been failed";
        fclose(asinc_result);
        fclose(sinc_result);
        return 0;
    }

    if (AreFilesEq(asinc_result, sinc_result)) {
        std::cout << "stress test has been passed";
    } else {
        std::cout << "stress test has been failed";
    }
    return 0;
}
