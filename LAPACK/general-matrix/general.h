#ifndef GENERAL_H
#define GENERAL_H

int create_matrix(float **matrix, int rows, int columns);
int auto_fill(float **matrix, int rows, int columns, int seed);
int show_matrix(float *matrix, int rows, int columns);
int symmetric_fill(float **matrix, int rows, int columns, int seed);
int pack_matrix(float **A, float **AP, int rows, int columns, int n);
int packed_symmetric_fill(float **matrix, int size, int seed);

#endif