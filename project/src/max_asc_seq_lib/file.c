// Created by nat-s.skv@mail.ru on 28.10.2021.
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <file.h>

#define ERR_OF_OPENING_FILE 1
#define ERR_OF_CLOSING_FILE 2
#define NULL_POINTER_PASS 3
#define FILE_ERR 4
#define END_OF_FILE 5
#define SCANF_ERR 6
#define MEMORY_ALLOCATION_ERR 7
#define MEMORY_UNMAP_ERR 8
#define MEMORY_MAP_ERR 9

static int errno_file = 1;

FILE *open_file(const char *name, const char *modes) {
    FILE *file = fopen(name, modes);
    if (!file) {
        errno_file = ERR_OF_OPENING_FILE;
        return NULL;
    }
    if (ferror(file)) {
        close_file(file);
        errno_file = FILE_ERR;
        return NULL;
    }
    return file;
}

int close_file(FILE *file) {
    if (!file) {
        return 0;
    }
    if (file == stdin || file == stdout || file == stderr) {
        return 0;
    }
    if (fclose(file)) {
        errno_file = ERR_OF_CLOSING_FILE;
        return 1;
    }
    return 0;
}

int load_arr_from_file(FILE *fin, int **array, size_t *size) {
    errno = 0;
    if (!array || !size) {
        errno_file = NULL_POINTER_PASS;
        return 1;
    }
    if (!fin) {
        *array = NULL;
        *size = 0u;
        errno_file = NULL_POINTER_PASS;
        return 1;
    }
    if (ferror(fin)) {
        *array = NULL;
        *size = 0u;
        errno_file = FILE_ERR;
        return 1;
    }
    if (feof(fin)) {
        *array = NULL;
        *size = 0u;
        errno_file = END_OF_FILE;
        return 1;
    }

    int fin_d = fileno(fin);
    struct stat st;
    fstat(fin_d, &st);

    size_t fin_size = st.st_size;

    char *region = (char *) mmap(NULL,
                                 fin_size,
                                 PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_POPULATE,
                                 fin_d,
                                 0);

    if (region == MAP_FAILED) {
        close(fin_d);
        errno_file = MEMORY_MAP_ERR;
        return 1;
    }

    int pos_in_region = 0;
    if (sscanf(region, "%zu%n", size, &pos_in_region) != 1 || errno == ERANGE) {
        *array = NULL;
        *size = 0u;
        if (feof(fin)) {
            errno_file = END_OF_FILE;
            return 1;
        }
        errno_file = SCANF_ERR;
        return 1;
    }
    if (!*size) {
        *array = NULL;
        return 0;
    }
    char *region_seek = region + pos_in_region;

    *array = (int *) malloc(*size * sizeof(int));
    if (!*array) {
        *size = 0u;
        errno_file = MEMORY_ALLOCATION_ERR;
        return 1;
    }

    for (size_t i = 0u; i < *size; ++i, region_seek += pos_in_region) {
        if (sscanf(region_seek, "%d%n", &((*array)[i]), &pos_in_region) != 1 || errno == ERANGE) {
            free(*array);
            *array = NULL;
            *size = 0u;
            if (feof(fin)) {
                errno_file = END_OF_FILE;
                return 1;
            }
            errno_file = SCANF_ERR;
            return 1;
        }
    }
    if (munmap(region, fin_size) != 0) {
        errno_file = MEMORY_UNMAP_ERR;
        return 1;
    }
    return 0;
}

/*
int load_arr_from_file(FILE *fin, int **array, size_t *size) {
    errno = 0;
    if (!array || !size) {
        errno_file = NULL_POINTER_PASS;
        return 1;
    }
    if (!fin) {
        *array = NULL;
        *size = 0u;
        errno_file = NULL_POINTER_PASS;
        return 1;
    }
    if (ferror(fin)) {
        *array = NULL;
        *size = 0u;
        errno_file = FILE_ERR;
        return 1;
    }
    if (feof(fin)) {
        *array = NULL;
        *size = 0u;
        errno_file = END_OF_FILE;
        return 1;
    }
    if (fscanf(fin, "%zu", size) != 1 || errno == ERANGE) {
        *array = NULL;
        *size = 0u;
        if (feof(fin)) {
            errno_file = END_OF_FILE;
            return 1;
        }
        errno_file = SCANF_ERR;
        return 1;
    }
    if (!*size) {
        *array = NULL;
        return 0;
    }
    *array = (int *) malloc(*size * sizeof(int));
    if (!*array) {
        *size = 0u;
        errno_file = MEMORY_ALLOCATION_ERR;
        return 1;
    }
    for (size_t i = 0u; i < *size; ++i) {
        if (fscanf(fin, "%d", &((*array)[i])) != 1 || errno == ERANGE) {
            free(*array);
            *array = NULL;
            *size = 0u;
            if (feof(fin)) {
                errno_file = END_OF_FILE;
                return 1;
            }
            errno_file = SCANF_ERR;
            return 1;
        }
    }
    return 0;
}
*/
void upload_arr_to_file(FILE *fout, int *array, size_t size) {
    if (!array || !fout) {
        errno_file = NULL_POINTER_PASS;
        return;
    }
    if (ferror(fout)) {
        errno_file = FILE_ERR;
        return;
    }
    fprintf(fout, "%zu", size);
    for (size_t i = 0u; i < size; ++i) {
        fprintf(fout, " %d", array[i]);
    }
}

void perror_file(const char *str) {
    fputs(__FILE__ ": ", stderr);
    if (str && str[0]) {
        fputs(str, stderr);
    }
    switch (errno_file) {
        case ERR_OF_OPENING_FILE:
            perror(": error of opening file");
            break;
        case ERR_OF_CLOSING_FILE:
            perror(": error of closing file");
            break;
        case SCANF_ERR:
            perror(": error of reading value from file");
            break;
        case MEMORY_ALLOCATION_ERR:
            perror(": error of allocation memory");
            break;
        case MEMORY_MAP_ERR:
            perror(": error of memory mapping");
            break;
        case MEMORY_UNMAP_ERR:
            perror(": error of memory unmapping");
            break;
        case FILE_ERR:
            fprintf(stderr, " :error inside file\n");
            break;
        case NULL_POINTER_PASS:
            fprintf(stderr, " :NULL pointer has been passed into a function\n");
            break;
        case END_OF_FILE:
            fprintf(stderr, ": end of file reached\n");
            break;
        default:
            break;
    }
}
