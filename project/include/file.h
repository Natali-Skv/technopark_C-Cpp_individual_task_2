// Created by nat-s.skv@mail.ru on 28.10.2021.
#ifndef TP_HW1_FILE_H
#define TP_HW1_FILE_H

FILE *open_file(const char *name, const char *modes);

int close_file(FILE *file);

int load_arr_from_file(FILE *fin, int **array, size_t *size);

void upload_arr_to_file(FILE *fout, int *array, size_t size);

void perror_file(const char* str);


#endif //TP_HW1_FILE_H
