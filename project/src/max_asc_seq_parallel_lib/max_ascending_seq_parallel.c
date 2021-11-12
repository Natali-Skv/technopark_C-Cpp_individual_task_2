// Copyright 2021 nat-s.skv@mail.ru
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <zconf.h>

#include <max_ascending_seq.h>

#define MIN_LEN_ONE_THREAD 1024u

typedef struct {
    const int *start_el;
    const int *end_el;
    size_t asc_seq_len_at_beg;
    size_t asc_seq_len_at_end;
    size_t max_asc_seq_len_at_mid;
} thread_result;

size_t get_max_asc_seq_len_one_thread(const int *array, size_t size);

void *process_array_block(void *);

size_t get_max_asc_seq_len(const int *array, size_t size) {
    if (!array) {
        return 0u;
    }
    if (!size) {
        return 0u;
    }

    size_t thread_num = (size_t) sysconf(_SC_NPROCESSORS_ONLN) < (size / MIN_LEN_ONE_THREAD) ?
                        (size_t) sysconf(_SC_NPROCESSORS_ONLN) : (size / MIN_LEN_ONE_THREAD);
    if (!thread_num) {
        thread_num = 1u;
    }
    if (thread_num == 1u) {
        return get_max_asc_seq_len_one_thread(array, size);
    }

    pthread_t threads[thread_num];
    thread_result thread_results[thread_num];

    size_t block_len = size / thread_num;
    for (size_t i = 0u; i < thread_num - 1u; ++i) {
        thread_results[i].start_el = array + i * block_len;
        thread_results[i].end_el = array + (i + 1) * block_len - 1u;
    }
    thread_results[thread_num - 1u].start_el = array + (thread_num - 1u) * block_len;
    thread_results[thread_num - 1u].end_el = array + (thread_num) * block_len - 1u + size % thread_num;

    for (size_t i = 0; i < thread_num; ++i) {
        if (pthread_create(&threads[i], NULL, process_array_block, (void *) &thread_results[i]) != EXIT_SUCCESS) {
            return 0u;
        }
    }

    for (size_t i = 0; i < thread_num; ++i) {
        pthread_join(threads[i], NULL);
    }

    size_t cross_block_asc_seq_len = thread_results[0u].asc_seq_len_at_end;
    size_t max_asc_seq_len = thread_results[0u].max_asc_seq_len_at_mid;
    for (size_t i = 1u; i < thread_num ; ++i) {
        if (max_asc_seq_len < thread_results[i].max_asc_seq_len_at_mid) {
            max_asc_seq_len = thread_results[i].max_asc_seq_len_at_mid;
        }
        if (*(thread_results[i-1u].end_el) < *(thread_results[i].start_el)) {
            if (thread_results[i].max_asc_seq_len_at_mid < block_len) {
                if (max_asc_seq_len < (cross_block_asc_seq_len + thread_results[i].asc_seq_len_at_beg)) {
                    max_asc_seq_len = cross_block_asc_seq_len + thread_results[i].asc_seq_len_at_beg;
                }
                cross_block_asc_seq_len = thread_results[i].asc_seq_len_at_end;
            } else {
                cross_block_asc_seq_len += thread_results[i].max_asc_seq_len_at_mid;
            }
        } else {
            if (max_asc_seq_len < cross_block_asc_seq_len) {
                max_asc_seq_len = cross_block_asc_seq_len;
            }
            cross_block_asc_seq_len = thread_results[i].asc_seq_len_at_end;
        }

    }

    if (max_asc_seq_len < cross_block_asc_seq_len) {
        max_asc_seq_len = cross_block_asc_seq_len;
    }
    return max_asc_seq_len;
}


void *process_array_block(void *args) {
    if (!args) {
        return EXIT_SUCCESS;
    }
    thread_result *block_data = (thread_result *) args;
    if (!block_data->start_el || !block_data->end_el) {
        return EXIT_SUCCESS;
    }
    size_t len = block_data->end_el - block_data->start_el + 1;
    if (len == 1u) {
        block_data->asc_seq_len_at_beg = 1u;
        block_data->max_asc_seq_len_at_mid = 1u;
        block_data->asc_seq_len_at_beg = 1u;
        return EXIT_SUCCESS;
    }
    const int *block = block_data->start_el;
    size_t i = 1u;
    block_data->asc_seq_len_at_beg = 1u;
    for (; i < len && block[i - 1u] < block[i]; ++i) {
        ++block_data->asc_seq_len_at_beg;
    }
    if (i == len) {
        block_data->max_asc_seq_len_at_mid = len;
        block_data->asc_seq_len_at_end = len;
        return EXIT_SUCCESS;
    }
    size_t end_asc_seq_at_beg = i;
    block_data->asc_seq_len_at_end = 1u;
    for (i = len - 1; i > end_asc_seq_at_beg && block[i - 1u] < block[i]; --i) {
        ++block_data->asc_seq_len_at_end;
    }

    block_data->max_asc_seq_len_at_mid = get_max_asc_seq_len_one_thread(block + end_asc_seq_at_beg,
                                                                        len - block_data->asc_seq_len_at_beg -
                                                                        block_data->asc_seq_len_at_end);
    if (block_data->asc_seq_len_at_beg > block_data->max_asc_seq_len_at_mid) {
        block_data->max_asc_seq_len_at_mid = block_data->asc_seq_len_at_beg;
    }
    if (block_data->asc_seq_len_at_end > block_data->max_asc_seq_len_at_mid) {
        block_data->max_asc_seq_len_at_mid = block_data->asc_seq_len_at_end;
    }
    return EXIT_SUCCESS;
}

size_t get_max_asc_seq_len_one_thread(const int *array, size_t size) {
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
