#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>
#include "alg_lin.h"

int main(int argc, const char *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 6) {
        printf("Use: %s <number_rows> <number_columns> <matrix_type> <ops> <seed>\n", argv[0]);
        return -1;
    }

    // Extraindo argumentos da execução
    lapack_int n_rows = atoi(argv[1]), n_columns = atoi(argv[2]);
    const char *MATRIX_TYPE = argv[3];
    const char *OPS = argv[4];
    const int RANDOM_SEED = atoi(argv[5]);

    // Validando argumentos
    int err_code = check_args(argc, n_rows, n_columns, MATRIX_TYPE, OPS, RANDOM_SEED);
    if(err_code)
        return err_code;

    lapack_int *ipiv = NULL;
    // Fluxo de tipo da matriz
    if(!strcmp(MATRIX_TYPE, "g")){
        // Criação de uma matriz A
        float *A = NULL;
        create_matrix(&A, n_rows, n_columns);

        // Preenchimento de uma general matrix
        auto_fill(&A, n_rows, n_columns, RANDOM_SEED);
        
        show_matrix(A, n_rows, n_columns, "A");

        // Fluxo de operações com a matriz geral
        if(strpbrk(OPS, "f")){

            // Fatoração de uma matriz A geral
            ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
            lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
            
            printf("LU info: %d\n", info_LU);
            show_matrix(A, n_rows, n_columns, "A factorized");

            if(strcmp(OPS, "f"))
            free(A);

        }
        if(strpbrk(OPS, "i")){

            // Inversão de uma matriz A
            float *inv_A = NULL;
            create_matrix(&inv_A, n_rows, n_columns);
            copy_matrix(&inv_A, &A, n_rows*n_columns);
            // show_matrix(inv_A, n_rows, n_columns, "inv_A");

            lapack_int info_inv = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);
            printf("inv info: %d\n", info_inv);

            show_matrix(inv_A, n_rows, n_columns, "A inversed");

            free(inv_A);

        }
        if(strpbrk(OPS, "s")){

            // RHS
            float *b = NULL;
            create_matrix(&b, n_rows, 1);
            auto_fill(&b, n_rows, 1, RANDOM_SEED);
            show_matrix(b, n_rows, 1, "b");

            // Ax = b
            lapack_int info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, b , 1);
            printf("solved system info: %d\n", info_solved_system);
            show_matrix(b, n_rows, 1, "x");

        }
    }

    else if(!strcmp(MATRIX_TYPE, "s")){
        // Criação de uma matriz AP
        float *AP = NULL;
        int size = n_rows*(n_rows+1)/2;
        create_matrix(&AP, 1, size);

        // Preenchimento de uma packed matrix
        packed_symmetric_fill(&AP, size, RANDOM_SEED);
        
        show_matrix(AP, 1, size, "AP");
    
        lapack_int* ipiv = NULL;
        // Fluxo de operações com a matriz simétrica
        if(strpbrk(OPS, "f")){

            // Fatoração de uma matriz AP simétrica
            ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
            lapack_int info_Bunch_Kaufman = LAPACKE_ssptrf(LAPACK_ROW_MAJOR, 'U', n_rows, AP, ipiv);
            
            printf("Bunch-Kaufman info: %d\n", info_Bunch_Kaufman);
            show_matrix(AP, 1, size, "AP factorized");

            if(strcmp(OPS, "f"))
                free(AP);

        }
        if(strpbrk(OPS, "i")){

            // Inversão de uma matriz A
            float *inv_AP = NULL;
            create_matrix(&inv_AP, n_rows, n_columns);
            copy_matrix(&inv_AP, &AP, n_rows*n_columns);
            // show_matrix(inv_AP, n_rows, n_columns, "inv_AP");

            lapack_int info_inv = LAPACKE_ssptri(LAPACK_ROW_MAJOR, 'U', n_rows, AP, ipiv);
            printf("inv info: %d\n", info_inv);

            show_matrix(inv_AP, 1, size, "AP inversed");

            free(inv_AP);

        }
        if(strpbrk(OPS, "s")){
            // REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR

            // RHS
            float *b = NULL;
            create_matrix(&b, n_rows, 1);
            auto_fill(&b, n_rows, 1, RANDOM_SEED);
            show_matrix(b, n_rows, 1, "b");

            // Ax = b
            lapack_int info_solved_system = LAPACKE_ssptrs(LAPACK_ROW_MAJOR, 'U', n_rows, 1, AP, ipiv , b, 1);
            printf("solved system info: %d\n", info_solved_system);
            show_matrix(b, n_rows, 1, "x");

            free(b);
        }
    }
    free(ipiv);

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}