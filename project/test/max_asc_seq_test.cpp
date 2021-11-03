// Created by nat-s.skv@mail.ru on 28.10.2021.

extern "C" {
#include <max_ascending_seq.h>
#include <file.h>
}

#include <gtest/gtest.h>
#include <cstring>
#include <fstream>

class FixtureLoadUpload : public ::testing::Test {
protected:

    void SetUp(const char *finPath, size_t expLen) {
        FILE *fin = open_file(finPath, "r");
        ASSERT_TRUE(fin);

        int *array = NULL;
        size_t size = 0u;
        load_arr_from_file(fin, &array, &size);
        EXPECT_EQ(expLen, get_max_asc_seq_len(array, size));

        free(array);
        close_file(fin);
    }

};

TEST_F(FixtureLoadUpload, GET_MAX_ASC_SEQ_LEN_10_OUT_OF_10) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_10/fin", 10u);
}

TEST_F(FixtureLoadUpload, GET_MAX_ASC_SEQ_LEN_14_OUT_OF_40) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/14_out_of_40/fin", 14u);
}

TEST_F(FixtureLoadUpload, GET_MAX_ASC_SEQ_LEN_1_OUT_OF_100) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/1_out_of_100/fin", 1u);
}

TEST_F(FixtureLoadUpload, GET_MAX_ASC_SEQ_LEN_16_OUT_OF_17) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/16_out_of_17/fin", 16u);
}

TEST_F(FixtureLoadUpload, GET_MAX_ASC_SEQ_LEN_1_OUT_OF_1) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/1_out_of_1/fin", 1u);
}

TEST_F(FixtureLoadUpload, GET_MAX_ASC_SEQ_LEN_10_OUT_OF_20) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_20/fin", 10u);
}

TEST_F(FixtureLoadUpload, GET_MAX_ASC_EMPTY) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/empty_array/fin", 0u);
}

TEST(GET_MAX_SEQ_LEN, INVALID_ARGS) {
    EXPECT_EQ(get_max_asc_seq_len(NULL, 100), 0u);
    int a;
    EXPECT_EQ(get_max_asc_seq_len(&a, 0), 0u);
}
