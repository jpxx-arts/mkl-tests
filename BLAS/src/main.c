#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>
#include "../include/alg_lin.h"

int main(int argc, char const *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 10){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <routine> <type> <matrix_type> <show> <seed>\n", argv[0]);
        return -1;
    }

    lapack_int A_rows = atoi(argv[1]), A_columns = atoi(argv[2]), B_rows = atoi(argv[3]), B_columns = atoi(argv[4]), RANDOM_SEED = atoi(argv[9]);
    const char *ROUTINE = argv[5], TYPE = argv[6][0], MATRIX_TYPE = argv[7][0], SHOW = argv[8][0];

    int err_code = check_args_BLAS(argv[0], A_rows, A_columns, B_rows, B_columns, ROUTINE);
    if(err_code)
        return err_code;

    if(!strcmp(ROUTINE, "vv")){

    }
    else if(!strcmp(ROUTINE, "mv")){
    
    }
    else if(!strcmp(ROUTINE, "mm")){
        switch (MATRIX_TYPE){
            case 'g': {
                switch(TYPE){
                    case 's': {
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

                        break;
                    }

                    case 'd': {
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

                        break;
                    }

                    case 'c': {
                        lapack_complex_float *A = NULL;
                        create_complex_float_matrix(&A, A_rows, A_columns);
                        
                        lapack_complex_float *B = NULL;
                        create_complex_float_matrix(&B, B_rows, B_columns);

                        lapack_complex_float *C = NULL;
                        create_complex_float_matrix(&C, A_rows, B_columns);
                    
                        auto_complex_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
                        if(SHOW == 's')
                            show_complex_float_matrix(A, A_rows, A_columns, "A");

                        auto_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                        if(SHOW == 's')
                            show_complex_float_matrix(B, B_rows, B_columns, "B");

                        lapack_complex_float alpha = {1.0, 1.0};
                        lapack_complex_float beta = {0.0, 0.0};

                        cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, &alpha, A, A_columns, B, B_rows, &beta, C, B_columns);
                        if(SHOW == 's')
                            show_complex_float_matrix(C, A_rows, B_columns, "C");
                            
                        break;
                    }

                    case 'z': {
                        lapack_complex_double *A = NULL;
                        create_complex_double_matrix(&A, A_rows, A_columns);
                        
                        lapack_complex_double *B = NULL;
                        create_complex_double_matrix(&B, B_rows, B_columns);

                        lapack_complex_double *C = NULL;
                        create_complex_double_matrix(&C, A_rows, B_columns);
                    
                        auto_complex_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
                        if(SHOW == 's')
                            show_complex_double_matrix(A, A_rows, A_columns, "A");

                        auto_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                        if(SHOW == 's')
                            show_complex_double_matrix(B, B_rows, B_columns, "B");

                            lapack_complex_double alpha = {1.0, 1.0};
                            lapack_complex_double beta = {0.0, 0.0};

                            cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, &alpha, A, A_columns, B, B_rows, &beta, C, B_columns);
                            if(SHOW == 's')
                                show_complex_double_matrix(C, A_rows, B_columns, "C");
                            
                        break;
                    }
                }

                break;
            }

            case 's': {
                switch(TYPE){
                    case 's': {
                        float *A = NULL;
                        create_float_matrix(&A, A_rows, A_columns);
                        
                        float *B = NULL;
                        create_float_matrix(&B, B_rows, B_columns);

                        float *C = NULL;
                        create_float_matrix(&C, A_rows, B_columns);
                        
                        symmetric_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
                        if(SHOW == 's')
                            show_float_matrix(A, A_rows, A_columns, "A");

                        symmetric_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                        if(SHOW == 's')
                            show_float_matrix(B, B_rows, B_columns, "B");

                        cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
                        if(SHOW == 's')
                            show_float_matrix(C, A_rows, B_columns, "C");

                        break;
                    }

                    case 'd': {
                        double *A = NULL;
                        create_double_matrix(&A, A_rows, A_columns);
                        
                        double *B = NULL;
                        create_double_matrix(&B, B_rows, B_columns);

                        double *C = NULL;
                        create_double_matrix(&C, A_rows, B_columns);
                    
                        symmetric_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
                        if(SHOW == 's')
                            show_double_matrix(A, A_rows, A_columns, "A");

                        symmetric_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                        if(SHOW == 's')
                            show_double_matrix(B, B_rows, B_columns, "B");

                        cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
                        if(SHOW == 's')
                            show_double_matrix(C, A_rows, B_columns, "C");

                        break;
                    }

                    case 'c': {
                        lapack_complex_float *A = NULL;
                        create_complex_float_matrix(&A, A_rows, A_columns);
                        
                        lapack_complex_float *B = NULL;
                        create_complex_float_matrix(&B, B_rows, B_columns);

                        lapack_complex_float *C = NULL;
                        create_complex_float_matrix(&C, A_rows, B_columns);
                    
                        symmetric_complex_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
                        if(SHOW == 's')
                            show_complex_float_matrix(A, A_rows, A_columns, "A");

                        symmetric_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                        if(SHOW == 's')
                            show_complex_float_matrix(B, B_rows, B_columns, "B");

                        lapack_complex_float alpha = {1.0, 1.0};
                        lapack_complex_float beta = {0.0, 0.0};

                        cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, &alpha, A, A_columns, B, B_rows, &beta, C, B_columns);
                        if(SHOW == 's')
                            show_complex_float_matrix(C, A_rows, B_columns, "C");
                            
                        break;
                    }

                    case 'z': {
                        lapack_complex_double *A = NULL;
                        create_complex_double_matrix(&A, A_rows, A_columns);
                        
                        lapack_complex_double *B = NULL;
                        create_complex_double_matrix(&B, B_rows, B_columns);

                        lapack_complex_double *C = NULL;
                        create_complex_double_matrix(&C, A_rows, B_columns);
                    
                        symmetric_complex_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
                        if(SHOW == 's')
                            show_complex_double_matrix(A, A_rows, A_columns, "A");

                        symmetric_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
                        if(SHOW == 's')
                            show_complex_double_matrix(B, B_rows, B_columns, "B");

                            lapack_complex_double alpha = {1.0, 1.0};
                            lapack_complex_double beta = {0.0, 0.0};

                            cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, &alpha, A, A_columns, B, B_rows, &beta, C, B_columns);
                            if(SHOW == 's')
                                show_complex_double_matrix(C, A_rows, B_columns, "C");
                            
                        break;
                    }
                }

                break;
            }
        }
    }

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}