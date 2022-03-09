// Copyright 2021 nat-s.skv@mail.ru
#include <stdio.h>

#include <max_ascending_seq.h>

size_t get_max_asc_seq_len(const int *array, size_t size) {
    if (!array) {
        return 0u;
    }
    if (!size) {
        return 0u;
    }
    size_t max_asc_len = 0u;
    size_t curr_seq_len = 1u;
    for (size_t i = 1u; i < size && max_asc_len < size / 2 + 1; ++i) {
        if (array[i] > array[i - 1]) {
            ++curr_seq_len;
            continue;
        }
        if (curr_seq_len > max_asc_len) {
            max_asc_len = curr_seq_len;
        }
        curr_seq_len = 1u;
    }
    if (curr_seq_len > max_asc_len) {
        max_asc_len = curr_seq_len;
    }
    return max_asc_len;
}
