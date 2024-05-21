#ifndef ALGLIN_H
#define ALGLIN_H

int check_args(int argc, const char *BIN, int rows, int columns, const char TYPE, const char *MATRIX_TYPE, const char *OPS, const int RANDOM_SEED);
int create_float_matrix(float **matrix, int rows, int columns);
int auto_float_fill(float **matrix, int rows, int columns, int seed);
int symmetric_float_fill(float **matrix, int rows, int columns, int seed);
int triangular_float_fill(float **matrix, int rows, int columns, int seed);
int show_float_matrix(float *matrix, int rows, int columns, char *matrix_name);
int show_int_matrix(int *matrix, int rows, int columns, char *matrix_name);
int show_float_packed_matrix(float *matrix, char uplo, int rows, int columns, char *matrix_name);
int pack_float_matrix(float **A, float **AP, int rows, int columns, int n);
int packed_float_fill(float **matrix, int size, int seed);
int copy_float_matrix(float **dest, float **source, int size);

int create_double_matrix(double **matrix, int rows, int columns);
int auto_double_fill(double **matrix, int rows, int columns, int seed);
int symmetric_double_fill(double **matrix, int rows, int columns, int seed);
int triangular_double_fill(double **matrix, int rows, int columns, int seed);
int show_double_matrix(double *matrix, int rows, int columns, char *matrix_name);
int show_double_packed_matrix(double *matrix, char uplo, int rows, int columns, char *matrix_name);
int pack_double_matrix(double **A, double **AP, int rows, int columns, int n);
int packed_double_fill(double **matrix, int size, int seed);
int copy_double_matrix(double **dest, double **source, int size);

#endif