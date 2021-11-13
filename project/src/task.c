// Copyright 2021 nat-s.skv@mail.ru
#include <getopt.h>
#include <stdio.h>
#include <unistd.h>

#include <task.h>
#include <file.h>
#include <max_ascending_seq.h>
#include <stdlib.h>
#include <time.h>

#define MICROSECOND_IN_SECOND 1000000
#define NANOSECOND_IN_MICROSECOND 1000

int get_fin_fout(int argc, char *argv[], FILE **fin, FILE **fout);

int run(int argc, char *argv[]) {
    FILE *fin = NULL;
    FILE *fout = NULL;
    if (get_fin_fout(argc, argv, &fin, &fout)) {
        return 1;
    }

    int *array = NULL;
    size_t size = 0u;
    if (load_arr_from_file(fin, &array, &size)) {
        perror_file("error of loading array from file");
        if (close_file(fin) || close_file(fout)) {
            perror_file(NULL);
        }
        return 1;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC_RAW, &start);

    fprintf(fout, "%zu", get_max_asc_seq_len(array, size));

    clock_gettime(CLOCK_MONOTONIC_RAW, &end);
    printf("working time:  %ld nanoseconds\n", (end.tv_sec - start.tv_sec) * MICROSECOND_IN_SECOND +
                                             (end.tv_nsec - start.tv_nsec) / NANOSECOND_IN_MICROSECOND);
    free(array);
    if (close_file(fin) || close_file(fout)) {
        perror_file(NULL);
    }
    return 0;
}

int get_fin_fout(int argc, char *argv[], FILE **fin, FILE **fout) {
    int opt;
    const char *opts = ":i:o:";
    *fin = NULL;
    *fout = NULL;

    while ((opt = getopt(argc, argv, opts)) != -1) {
        switch (opt) {
            case 'i':
                *fin = open_file(optarg, "r");
                if (!*fin) {
                    perror_file(optarg);
                    return 1;
                }
                break;
            case 'o':
                *fout = open_file(optarg, "w");
                if (!*fout) {
                    perror_file(optarg);
                    return 1;
                }
                break;
            case 'h':
                printf("Usage: %s [-i input_file|-o output_file] <args>\n", argv[0]);
                break;
            case '?':
                printf("Unknown option: %c\n", optopt);
                break;
            case ':':
                printf("Missing arg for %c\n", optopt);
                break;
            default:
                break;
        }
    }

    if (optind < argc) {
        printf("\nThe following arguments will not be processed\n");
        while (optind < argc) {
            printf("%s\n", argv[optind]);
            optind++;
        }
    }

    if (!*fin) {
        *fin = stdin;
    }
    if (!*fout) {
        *fout = stdout;
    }
    return 0;
}
