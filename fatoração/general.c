#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

int create_matrix(float **matrix, int rows, int columns);
void auto_fill(float **matrix, int rows, int columns, int seed);
void show_matrix(float *matrix, int rows, int columns);

int main(int argc, char const *argv[]){
    if(argc != 3) {
        printf("Use: %s <number_rows> <number_columns>\n", argv[0]);
        return -1;
    }

    lapack_int n_rows = atoi(argv[1]), n_columns = atoi(argv[2]);

    float *matrix = (float *) malloc(n_rows*n_columns*sizeof(float));
    auto_fill(&matrix, n_rows, n_columns, 2);
    printf("\n");
    show_matrix(matrix, n_rows, n_columns);

    lapack_int *ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
    lapack_int info = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, matrix, n_columns, ipiv);

    printf("info: %d\n", info);
    show_matrix(matrix, n_rows, n_columns);

    // Fazendo RHS
    float *b = (float *) malloc(n_rows*sizeof(float));
    auto_fill(&b, n_rows, 1, 1);
    lapack_int info1 = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, matrix, n_columns, ipiv, b , 1);

    show_matrix(b, n_rows, 1);

    free(ipiv);
    free(matrix);
    return 0;
}

int create_matrix(float **matrix, int rows, int columns){
    (*matrix) = (float *) malloc(rows*columns * sizeof(int));
    if ((*matrix) == NULL) {
        printf("Allocation error");
        free(*matrix);
        return -1;
    }

    return 0;
}

void auto_fill(float **matrix, int rows, int columns, int seed){
    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            (*matrix)[j + (i*columns)] = i+j;
        }
    }
}

void show_matrix(float *matrix, int rows, int columns){
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("%.2f ", matrix[j + (i*columns)]);
        }
        printf("\n");
    }
    printf("\n");
}