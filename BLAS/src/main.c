#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>

#define RANDOM_SEED 1

int create_matrix(float **matrix, int rows, int columns);
int auto_fill(float **matrix, int rows, int columns, int seed);
int show_matrix(float *matrix, int rows, int columns);

int main(int argc, char const *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 9){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <allocation> <op> <show> <seed>\n", argv[0]);
        return -1;
    }

    lapack_int A_rows = atoi(argv[1]), A_columns = atoi(argv[2]), B_rows = atoi(argv[3]), B_columns = atoi(argv[4]); int RANDOM_SEED = atoi(argv[8]);
    const char *OP = argv[6]; const char SHOW = argv[7][0];

    int err_code = check_args(argc, argv[0], n_rows, n_columns, TYPE, MATRIX_TYPE, OP, RANDOM_SEED, "BLAS");
    if(err_code)
        return err_code;

    float *A = (float *) malloc(A_rows*A_columns*sizeof(float));
    auto_fill(&A, A_rows, A_columns, RANDOM_SEED);
    printf("Matrix A:\n");
    show_matrix(A, A_rows, A_columns);

    float *B = (float *) malloc(B_rows*B_columns*sizeof(float));
    auto_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    printf("Matrix B:\n");
    show_matrix(B, B_rows, B_columns);

    float *C = (float *) malloc(A_rows*B_columns*sizeof(float));
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
    printf("Matrix C:\n");
    show_matrix(C, A_rows, B_columns);

    free(C);
    free(B);
    free(A);

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