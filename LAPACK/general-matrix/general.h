#ifndef GENERAL_H
#define GENERAL_H

int check_args(int argc, int rows, int columns, const char *MATRIX_TYPE, const char *OPS, const int RANDOM_SEED);
int create_matrix(float **matrix, int rows, int columns);
int auto_fill(float **matrix, int rows, int columns, int seed);
int show_matrix(float *matrix, int rows, int columns, char *matrix_name);
int symmetric_fill(float **matrix, int rows, int columns, int seed);
int pack_matrix(float **A, float **AP, int rows, int columns, int n);
int packed_symmetric_fill(float **matrix, int size, int seed);
int copy_matrix(float **dest, float **source, int size);

#endif