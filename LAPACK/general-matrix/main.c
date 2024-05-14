#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>
#include "general.h"

int main(int argc, const char *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    lapack_int n_rows = atoi(argv[1]), n_columns = atoi(argv[2]);
    const char *MATRIX_TYPE = argv[3];
    const char *OPS = argv[4];
    const int RANDOM_SEED = atoi(argv[5]);

    int err_code = check_args(argc, argv[0], MATRIX_TYPE, OPS, RANDOM_SEED);
    if(err_code)
        return err_code;

    // Fluxo de tipo da matriz
    if(!strcmp(MATRIX_TYPE, "g")){
        // Criação de uma matriz A
        float *A = NULL;
        create_matrix(&A, n_rows, n_columns);

        // Preenchimento de uma general matrix
        auto_fill(&A, n_rows, n_columns, RANDOM_SEED);
        
        show_matrix(A, n_rows, n_columns, "A");
    
        // Fluxo de operações com a matriz
        if(strpbrk(OPS, "f")){
            //Fatoração de uma matriz A geral
            lapack_int *ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
            lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
            
            printf("LU info: %d\n", info_LU);
            show_matrix(A, n_rows, n_columns, "A factorized");
        }else if(strpbrk(OPS, "i")){

        }else if(strpbrk(OPS, "s")){

        }
        
    }else if(!strcmp(MATRIX_TYPE, "s")){
        // Criação de uma matriz AP
        float *AP = NULL;
        int size = n_rows*(n_rows+1)/2;
        create_matrix(&AP, 1, size);

        // Preenchimento de uma packed matrix
        packed_symmetric_fill(&AP, size, RANDOM_SEED);
        
        show_matrix(AP, n_rows, n_columns, "AP");
    
        //Fatoração de uma matriz A simétrica
    }


    // //Inversão de uma matriz A
    // float *inv_A = (float *) malloc(n_rows*n_columns*sizeof(float));
    // memcpy(inv_A, A, n_rows*n_columns);
    // lapack_int info_inv = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);
    // printf("inv info: %d\n", info_inv);
    // printf("Inversed A:\n");
    // show_matrix(inv_A, n_rows, n_columns);

    // //RHS
    // float *b = (float *) malloc(n_rows*sizeof(float));
    // auto_fill(&b, n_rows, 1, RANDOM_SEED);
    // printf("Matrix b:\n");
    // show_matrix(b, n_rows, 1);

    // //Ax = b
    // lapack_int info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, b , 1);
    // printf("solved system info: %d\n", info_solved_system);
    // printf("Matrix x:\n");
    // show_matrix(b, n_rows, 1);

    // free(b);
    // free(ipiv);
    // free(inv_A);
    // free(A);

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}