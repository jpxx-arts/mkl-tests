#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>
#include "../include/alg_lin.h"

int main(int argc, char const *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 11){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <allocation> <routine> <type> <matrix_type> <show> <seed>\n", argv[0]);
        return -1;
    }

    lapack_int A_rows = atoi(argv[1]), A_columns = atoi(argv[2]), B_rows = atoi(argv[3]), B_columns = atoi(argv[4]), RANDOM_SEED = atoi(argv[10]);
    const char ALLOCATION = argv[5][0], *ROUTINE = argv[6], TYPE = argv[7][0], MATRIX_TYPE = argv[8][0], SHOW = argv[9][0];

    int err_code = check_args_BLAS(argv[0], A_rows, A_columns, B_rows, B_columns, ROUTINE);
    if(err_code)
        return err_code;

    if(!strcmp(ROUTINE, "vv")){

    }
    else if(!strcmp(ROUTINE, "mv")){
    
    }
    else if(!strcmp(ROUTINE, "mm")){
        if(MATRIX_TYPE == 'g'){
            if(TYPE == 's'){
                if(ALLOCATION == 's'){
                    float A[A_rows*A_columns];
                    float B[B_rows*B_columns];
                    float C[A_rows*B_columns];

                    auto_float_static_fill(A, A_rows, A_columns, RANDOM_SEED);
                    if(SHOW == 's')
                        show_float_matrix(A, A_rows, A_columns, "A");

                    auto_float_static_fill(B, B_rows, B_columns, RANDOM_SEED + 1);
                    if(SHOW == 's')
                        show_float_matrix(B, B_rows, B_columns, "B");

                    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
                    if(SHOW == 's')
                        show_float_matrix(C, A_rows, B_columns, "C");
                }
                else if(ALLOCATION == 'd'){
                    float *A = NULL;
                    create_float_matrix(&A, A_rows, A_columns);
                    
                    float *B = NULL;
                    create_float_matrix(&B, B_rows, B_columns);

                    float *C = NULL;
                    create_float_matrix(&C, A_rows, B_columns);
                    
                    auto_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
                    if(SHOW == 's')
                        show_float_matrix(A, A_rows, A_columns, "A");

                    auto_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                    if(SHOW == 's')
                        show_float_matrix(B, B_rows, B_columns, "B");

                    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
                    if(SHOW == 's')
                        show_float_matrix(C, A_rows, B_columns, "C");
                }
            }

            else if(TYPE == 'd'){
                if(ALLOCATION == 's'){
                    double A[A_rows*A_columns];
                    double B[B_rows*B_columns];
                    double C[A_rows*B_columns];

                    auto_double_static_fill(A, A_rows, A_columns, RANDOM_SEED);
                    if(SHOW == 's')
                        show_double_matrix(A, A_rows, A_columns, "A");

                    auto_double_static_fill(B, B_rows, B_columns, RANDOM_SEED + 1);
                    if(SHOW == 's')
                        show_double_matrix(B, B_rows, B_columns, "B");

                    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
                    if(SHOW == 's')
                        show_double_matrix(C, A_rows, B_columns, "C");
                }
                else if(ALLOCATION == 'd'){
                    double *A = NULL;
                    create_double_matrix(&A, A_rows, A_columns);
                    
                    double *B = NULL;
                    create_double_matrix(&B, B_rows, B_columns);

                    double *C = NULL;
                    create_double_matrix(&C, A_rows, B_columns);
                
                    auto_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
                    if(SHOW == 's')
                        show_double_matrix(A, A_rows, A_columns, "A");

                    auto_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                    if(SHOW == 's')
                        show_double_matrix(B, B_rows, B_columns, "B");

                    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
                    if(SHOW == 's')
                        show_double_matrix(C, A_rows, B_columns, "C");
                }
            }
        }
    }

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}
