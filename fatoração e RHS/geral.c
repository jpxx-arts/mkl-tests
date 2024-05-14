#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

#define RANDOM_SEED 1

int create_matrix(float **matrix, int rows, int columns);
int auto_fill(float **matrix, int rows, int columns, int seed);
int show_matrix(float *matrix, int rows, int columns);

int main(int argc, char const *argv[]){
    if(argc != 3) {
        printf("Use: %s <number_rows> <number_columns>\n", argv[0]);
        return -1;
    }

    lapack_int n_rows = atoi(argv[1]), n_columns = atoi(argv[2]);

    // Fatoração de uma matriz A
    float *A = (float *) malloc(n_rows*n_columns*sizeof(float));
    auto_fill(&A, n_rows, n_columns, RANDOM_SEED);
    printf("Matrix A:\n");
    show_matrix(A, n_rows, n_columns);

    lapack_int *ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
    lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);

    printf("LU info: %d\n", info_LU);
    printf("Matrix A factorized:\n");
    show_matrix(A, n_rows, n_columns);

    // RHS
    float *b = (float *) malloc(n_rows*sizeof(float));
    auto_fill(&b, n_rows, 1, RANDOM_SEED);
    printf("Matrix b:\n");
    show_matrix(b, n_rows, 1);

    // Ax = b
    lapack_int info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, b , 1);
    printf("solved system info: %d\n", info_solved_system);
    printf("Matrix x:\n");
    show_matrix(b, n_rows, 1);

    free(ipiv);
    free(A);
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

int auto_fill(float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            (*matrix)[j + (i*columns)] = rand()%10;
        }
    }

    return 0;
}

int show_matrix(float *matrix, int rows, int columns){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("%.2f ", matrix[j + (i*columns)]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}