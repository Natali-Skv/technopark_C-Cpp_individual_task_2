// Copyright 2021 nat-s.skv@mail.ru
#ifndef PROJECT_INCLUDE_FILE_H_
#define PROJECT_INCLUDE_FILE_H_

FILE *open_file(const char *name, const char *modes);

int close_file(FILE *file);

int load_arr_from_file(FILE *fin, int **array, size_t *size);

void upload_arr_to_file(FILE *fout, int *array, size_t size);

void perror_file(const char* str);


#endif  // PROJECT_INCLUDE_FILE_H_
