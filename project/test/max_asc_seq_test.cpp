// Copyright 2021 nat-s.skv@mail.ru
extern "C" {
#include <max_ascending_seq.h>
#include <file.h>
}

#include <gtest/gtest.h>
#include <fstream>
#include <cstring>

static std::ofstream result_file(RESULT_FILE);

class Fixture : public ::testing::Test {
protected:
    size_t SetUp(const char *finPath, size_t expLen) {
        FILE *fin = open_file(finPath, "r");
        if (!fin) {
            return 0;
        }

        int *array = NULL;
        size_t size = 0u;
        load_arr_from_file(fin, &array, &size);
        size_t max_asc_len = get_max_asc_seq_len(array, size);

        EXPECT_EQ(expLen, max_asc_len);

        free(array);
        close_file(fin);
        return max_asc_len;
    }
};

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_10_OUT_OF_10) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_10/fin", 10u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_14_OUT_OF_40) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/14_out_of_40/fin", 14u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_1_OUT_OF_100) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/1_out_of_100/fin", 1u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_16_OUT_OF_17) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/16_out_of_17/fin", 16u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_1_OUT_OF_1) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/1_out_of_1/fin", 1u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_10_OUT_OF_20) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_20/fin", 10u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_11_OUT_OF_10mln) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/11_out_of_10mln/fin", 11u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_10_OUT_OF_10mln) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_10mln/fin", 10u) << ' ';
}

TEST_F(Fixture, GET_MAX_ASC_SEQ_LEN_10_OUT_OF_25mln) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_25mln/fin", 10u) << ' ';
    result_file.close();
}


TEST_F(Fixture, GET_MAX_ASC_EMPTY) {
    result_file << Fixture::SetUp(PROJECT_PATH "test/fin_fout/empty_array/fin", 0u) << ' ';
}

TEST(GET_MAX_SEQ_LEN, INVALID_ARGS) {
    size_t max_asc_len = get_max_asc_seq_len(NULL, 100);
    result_file << max_asc_len << ' ';
    EXPECT_EQ(max_asc_len, 0u);
    int a;
    max_asc_len = get_max_asc_seq_len(&a, 0);
    result_file << max_asc_len << ' ';
    EXPECT_EQ(max_asc_len, 0u);

    result_file.close();
}
