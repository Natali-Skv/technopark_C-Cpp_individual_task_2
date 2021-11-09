// Copyright 2021 nat-s.skv@mail.ru

#include <gtest/gtest.h>
#include <fstream>

int main(int argc, char **argv) {
    std::ofstream ofs("ofstream.txt");
    if (!ofs.bad())
    {
        ofs.close();
        return 0;
    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
