#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

#define RANDOM_SEED 1

int create_matrix(float **matrix, int rows, int columns);
int auto_fill(float **matrix, int rows, int columns, int seed);
int show_matrix(float *matrix, int rows, int columns);
int symmetric_fill(float **matrix, int rows, int columns, int seed);
int pack_matrix(float **A, float **AP, int rows, int columns, int n);

int main(int argc, char const *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 2) {
        printf("Use: %s <dimension>\n", argv[0]);
        return -1;
    }

    lapack_int n = atoi(argv[1]);

    // Fatoração de uma matriz A
    float *A = (float *) malloc(n*n*sizeof(float));
    symmetric_fill(&A, n, n, RANDOM_SEED);
    printf("Matrix A:\n");
    show_matrix(A, n, n);

    float *AP = (float *) malloc(n*(n+1)/2*sizeof(float));
    pack_matrix(&A, &AP, n, n, n);
    // printf("Matrix AP:\n");
    // for(int i = 0; i < n*(n+1)/2; i++){
    //     printf("%f ", AP[i]);
    // }

    lapack_int info_LU = LAPACKE_spotrf(LAPACK_ROW_MAJOR, 'U', n, AP, n);

    printf("LU info: %d\n", info_LU);
    printf("Matrix AP factorized:\n");
    show_matrix(AP, n, n);

    // // RHS
    // float *b = (float *) malloc(n*sizeof(float));
    // auto_fill(&b, n, 1, RANDOM_SEED);
    // printf("Matrix b:\n");
    // show_matrix(b, n, 1);

    // // Ax = b
    // lapack_int info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n, 1, A, n, ipiv, b , 1);
    // printf("solved system info: %d\n", info_solved_system);
    // printf("Matrix x:\n");
    // show_matrix(b, n, 1);

    // free(b);
    // free(ipiv);
    // free(A);

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

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

int symmetric_fill(float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){           
            if(j > i){
                (*matrix)[i*rows + j] = rand()%10;
            }
        }
    }

    return 0;
}

int pack_matrix(float **A, float **AP, int rows, int columns, int n){
    if(A == NULL || AP == NULL){
        printf("One or more empty matrices");
        return -1;
    }
    if(rows != columns){
        printf("The matrix must be square");
        return -2;
    }
    if(rows != n){
        printf("Incompatible sizes");
        return -3;
    }

    // int k;
    // for(int i = 1; i <= rows; i++){
    //     for(int j = 1; j <= columns; j++){
    //         if(i <= j){
    //             k = (j-1) + (i-1)*(2*n - i)/2;
    //             printf("%d", k);
    //             (*AP)[k] = (*A)[(i-1)*n + (j-1)];
    //         }
    //     }
    // }

    for (int i = 0, k = 0; i < n; i++) {
        for (int j = i; j < n; j++) {
            AP[k++] = A[i*n + j];
        }
    }

    return 0;
}