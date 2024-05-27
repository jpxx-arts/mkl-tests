#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>
#include "../include/alg_lin.h"

#define DEFINED_MULTIPLICATION_ARGS A_rows, A_columns, B_rows, B_columns, RANDOM_SEED, SHOW

int main(int argc, char const *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 10){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <routine> <type> <matrix_type> <show> <seed>\n", argv[0]);
        return -1;
    }

    lapack_int A_rows = atoi(argv[1]), A_columns = atoi(argv[2]), B_rows = atoi(argv[3]), B_columns = atoi(argv[4]);
    const char *ROUTINE = argv[5], TYPE = argv[6][0], MATRIX_TYPE = argv[7][0], SHOW = argv[8][0];
    const int RANDOM_SEED = atoi(argv[9]);

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
                        const float alpha = 1.0;
                        const float beta = 0.0;

                        multiply_float_general_matrices(DEFINED_MULTIPLICATION_ARGS, alpha, beta);

                        break;
                    }

                    case 'd': {
                        const double alpha = 1.0;
                        const double beta = 0.0;

                        multiply_double_general_matrices(DEFINED_MULTIPLICATION_ARGS, alpha, beta);
                        break;
                    }

                    case 'c': {
                        MKL_Complex8 alpha = {1.0, 1.0};
                        MKL_Complex8 beta = {0.0, 0.0};

                        multiply_complex_float_general_matrices(DEFINED_MULTIPLICATION_ARGS, &alpha, &beta);

                        break;
                    }

                    case 'z': {
                        MKL_Complex16 alpha = {1.0, 1.0};
                        MKL_Complex16 beta = {0.0, 0.0};

                        multiply_complex_double_general_matrices(DEFINED_MULTIPLICATION_ARGS, &alpha, &beta);

                        break;
                    }
                }

                break;
            }

            case 's': {
                switch(TYPE){
                    case 's': {
                        const float alpha = 1.0;
                        const float beta = 0.0;

                        multiply_float_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, alpha, beta);

                        break;
                    }

                    case 'd': {
                        const double alpha = 1.0;
                        const double beta = 0.0;

                        multiply_double_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, alpha, beta);
                        break;
                    }

                    case 'c': {
                        MKL_Complex8 alpha = {1.0, 1.0};
                        MKL_Complex8 beta = {0.0, 0.0};

                        multiply_complex_float_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, &alpha, &beta);

                        break;
                    }

                    case 'z': {
                        MKL_Complex16 alpha = {1.0, 1.0};
                        MKL_Complex16 beta = {0.0, 0.0};

                        multiply_complex_double_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, &alpha, &beta);

                        break;
                    }
                }

                break;
            }

            case 'e': {
                int non_zeros_number;
                switch(TYPE){
                    case 's': {
                        const float alpha = 1.0;
                        const float beta = 0.0;

                        multiply_sparse_float_general_matrices(DEFINED_MULTIPLICATION_ARGS, alpha, beta);

                        break;
                    }

                    case 'd': {
                        const double alpha = 1.0;
                        const double beta = 0.0;

                        multiply_double_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, alpha, beta);
                        break;
                    }

                    case 'c': {
                        MKL_Complex8 alpha = {1.0, 1.0};
                        MKL_Complex8 beta = {0.0, 0.0};

                        multiply_complex_float_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, &alpha, &beta);

                        break;
                    }

                    case 'z': {
                        MKL_Complex16 alpha = {1.0, 1.0};
                        MKL_Complex16 beta = {0.0, 0.0};

                        multiply_complex_double_symmetric_matrices(DEFINED_MULTIPLICATION_ARGS, &alpha, &beta);

                        break;
                    }
                }
            } 
        }
    }

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}
