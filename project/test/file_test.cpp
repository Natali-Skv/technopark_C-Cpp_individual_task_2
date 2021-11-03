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
    static const int bufSize = 100;

    void SetUp(const char *finPath, const char *foutPath) {
        FILE *fin = open_file(finPath, "r");
        FILE *fout = open_file(foutPath, "w+");
        ASSERT_TRUE(fin && fout);

        int *array = NULL;
        size_t size = 0u;
        load_arr_from_file(fin, &array, &size);
        upload_arr_to_file(fout, array, size);

        CompareFiles(fin, fout);

        free(array);
        fclose(fin);
        fclose(fout);
    }

    void CompareFiles(FILE *f1, FILE *f2) {
        ASSERT_TRUE(f1 && f2);
        ASSERT_FALSE(ferror(f1) || ferror(f2));
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
            EXPECT_EQ(std::memcmp(buf1, buf2, strlen(buf1)), 0);
        }
        EXPECT_TRUE(feof(f1) && feof(f2));
    }
};


TEST_F(FixtureLoadUpload, LOAD_UPLOAD_LEN_10) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_10/fin",
                             PROJECT_PATH "test/fin_fout/10_out_of_10/fout");
}

TEST_F(FixtureLoadUpload, LOAD_UPLOAD_LEN_20) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/10_out_of_20/fin",
                             PROJECT_PATH "test/fin_fout/10_out_of_20/fout");
}

TEST_F(FixtureLoadUpload, LOAD_UPLOAD_LEN_1) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/1_out_of_1/fin",
                             PROJECT_PATH "test/fin_fout/1_out_of_1/fout");
}

TEST_F(FixtureLoadUpload, LOAD_UPLOAD_LEN_100) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/1_out_of_100/fin",
                             PROJECT_PATH "test/fin_fout/1_out_of_100/fout");
}

TEST_F(FixtureLoadUpload, LOAD_UPLOAD_LEN_40) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/14_out_of_40/fin",
                             PROJECT_PATH "test/fin_fout/14_out_of_40/fout");
}

TEST_F(FixtureLoadUpload, LOAD_UPLOAD_LEN_17) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/16_out_of_17/fin",
                             PROJECT_PATH "test/fin_fout/16_out_of_17/fout");
}

TEST_F(FixtureLoadUpload, LOAD_UPLOAD_EMPTY_FILE) {
    FixtureLoadUpload::SetUp(PROJECT_PATH "test/fin_fout/empty_array/fin",
                             PROJECT_PATH "test/fin_fout/empty_array/fout");
}


TEST(LOAD, INVALID_ARGS) {
    size_t size = 0u;
    int *array = NULL;
    EXPECT_EQ(load_arr_from_file(NULL, &array, &size), 1);
    EXPECT_EQ(load_arr_from_file(stdin, &array, NULL), 1);
    EXPECT_EQ(load_arr_from_file(stdin, NULL, &size), 1);

    FILE *fout = open_file(PROJECT_PATH "test/fin_fout/invalid_args/erange_size", "w");
    ASSERT_TRUE(fout);
    // fout открыт только на запись
    EXPECT_EQ(load_arr_from_file(fout, &array, &size), 1);
}

TEST(LOAD, ERANGE) {
    size_t size = 0u;
    int *array = NULL;
    FILE *fin = open_file(PROJECT_PATH "test/fin_fout/invalid_args/bigger_size", "r");
    EXPECT_EQ(load_arr_from_file(fin, &array, &size), 1);
    EXPECT_EQ(array, (int *) NULL);
    EXPECT_EQ(size, 0u);
    close_file(fin);

    fin = open_file(PROJECT_PATH "test/fin_fout/invalid_args/erange_size", "r");
    EXPECT_EQ(load_arr_from_file(fin, &array, &size), 1);
    EXPECT_EQ(array, (int *) NULL);
    EXPECT_EQ(size, 0u);
    close_file(fin);

    fin = open_file(PROJECT_PATH "test/fin_fout/invalid_args/erange_element", "r");
    EXPECT_EQ(load_arr_from_file(fin, &array, &size), 1);
    EXPECT_EQ(array, (int *) NULL);
    EXPECT_EQ(size, 0u);
    close_file(fin);
}

TEST(OPEN_CLOSE, INVALID_ARGS) {
    EXPECT_EQ(open_file("none", "r"), (FILE *) NULL);
    EXPECT_EQ(open_file(PROJECT_PATH "test/fin_fout/invalid_args/erange_size", "none"), (FILE *) NULL);
    EXPECT_EQ(open_file(PROJECT_PATH "test/fin_fout/invalid_args/mod_555", "w"), (FILE *) NULL);

    EXPECT_EQ(close_file(stdin), 0);
    EXPECT_EQ(close_file(NULL), 0);
}
