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
    int n = n_rows;

    // Fluxo de tipo da matriz:

    // Para matrizes gerais
    if(!strcmp(MATRIX_TYPE, "g")){

        // Criação de uma matriz A
        float *A = NULL;
        create_float_matrix(&A, n_rows, n_columns);

        // Preenchimento de uma matriz geral
        auto_fill(&A, n_rows, n_columns, RANDOM_SEED);
        
        show_float_matrix(A, n_rows, n_columns, "A");

        // Fluxo de operações com a matriz geral
        if(strpbrk(OPS, "f")){

            // Fatoração de uma matriz A geral
            ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
            lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
            
            printf("LU info: %d\n", info_LU);
            show_float_matrix(A, n_rows, n_columns, "A factorized");

            show_int_matrix(ipiv, 1, n_columns, "ipiv");

            if(!strcmp(OPS, "f"))
                free(A);

        }
        if(strpbrk(OPS, "i")){
            if(!strpbrk(OPS, "f")){
                printf("To do this operation the matrix must be factorized\n");
                return -4;
            }

            // Inversão de uma matriz A
            float *inv_A = NULL;
            create_float_matrix(&inv_A, n_rows, n_columns);
            copy_matrix(&inv_A, &A, n_rows*n_columns);

            lapack_int info_inv = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

            printf("inv info: %d\n", info_inv);
            show_float_matrix(inv_A, n_rows, n_columns, "A inversed");

            free(inv_A);

        }
        if(strpbrk(OPS, "s")){
            if(!strpbrk(OPS, "f")){
                printf("To do this operation the matrix must be factorized\n");
                return -4;
            }

            // RHS
            float *b = NULL;
            create_float_matrix(&b, n_rows, 1);
            auto_fill(&b, n_rows, 1, RANDOM_SEED);
            show_float_matrix(b, n_rows, 1, "b");

            // Ax = b
            float *x = NULL;
            create_float_matrix(&x, n_rows, 1);
            copy_matrix(&x, &b, n_rows);

            lapack_int info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
            
            printf("solved system info: %d\n", info_solved_system);
            show_float_matrix(x, n_rows, 1, "x");

            free(x);
            free(b);
        }
    }

    // Para matrizes simétricas empacotadas
    else if(!strcmp(MATRIX_TYPE, "s")){

        // Criação de uma matriz AP
        float *AP = NULL;
        int size = n*(n+1)/2;
        create_float_matrix(&AP, 1, size);

        // Preenchimento de uma packed matrix
        packed_fill(&AP, size, RANDOM_SEED);
        
        show_float_packed_matrix(AP, 'N', n_rows, n_columns, "AP");
        // show_float_matrix(AP, 1, size, "AP");
    
        // Fluxo de operações com a matriz simétrica
        if(strpbrk(OPS, "f")){

            // Fatoração de uma matriz AP simétrica
            ipiv = (lapack_int *) malloc(n*sizeof(lapack_int));
            lapack_int info_Bunch_Kaufman = LAPACKE_ssptrf(LAPACK_ROW_MAJOR, 'U', n, AP, ipiv);
            
            printf("Bunch-Kaufman info: %d\n", info_Bunch_Kaufman);
            show_float_matrix(AP, 1, size, "AP factorized");

            if(!strcmp(OPS, "f"))
                free(AP);

        }
        if(strpbrk(OPS, "i")){
            if(!strpbrk(OPS, "f")){
                printf("To do this operation the matrix must be factorized\n");
                return -4;
            }

            // Inversão de uma matriz A
            float *inv_AP = NULL;
            create_float_matrix(&inv_AP, 1, size);
            copy_matrix(&inv_AP, &AP, size);

            lapack_int info_inv = LAPACKE_ssptri(LAPACK_ROW_MAJOR, 'U', n, inv_AP, ipiv);

            printf("inv info: %d\n", info_inv);
            show_float_packed_matrix(inv_AP, 'N', n_rows, n_columns, "AP inversed");

            free(inv_AP);

        }
        if(strpbrk(OPS, "s")){
            if(!strpbrk(OPS, "f")){
                printf("To do this operation the matrix must be factorized\n");
                return -4;
            }

            // RHS
            float *b = NULL;
            create_float_matrix(&b, n, 1);
            auto_fill(&b, n, 1, RANDOM_SEED);
            show_float_matrix(b, n, 1, "b");

            // Ax = b
            float *x = NULL;
            create_float_matrix(&x, n_rows, 1);
            copy_matrix(&x, &b, n_rows);

            lapack_int info_solved_system = LAPACKE_ssptrs(LAPACK_ROW_MAJOR, 'U', n, 1, AP, ipiv , x, 1);

            printf("solved system info: %d\n", info_solved_system);
            show_float_matrix(x, n, 1, "x");

            free(x);
            free(b);

        }
    }

    // Para matrizes tringulares empacotadas
    else if(!strcmp(MATRIX_TYPE, "t")){

        // Criação de uma matriz AP
        float *AP = NULL;
        int size = n*(n+1)/2;
        create_float_matrix(&AP, 1, size);

        // Preenchimento de uma packed matrix
        packed_fill(&AP, size, RANDOM_SEED);
        
        show_float_packed_matrix(AP, 'U', n_rows, n_columns, "AP");
    
        // Fluxo de operações com a matriz simétrica
        if(strpbrk(OPS, "i")){

            // Inversão de uma matriz A
            float *inv_AP = NULL;
            create_float_matrix(&inv_AP, 1, size);
            copy_matrix(&inv_AP, &AP, size);

            for(int i = 0; i < size; i++){
                printf("%f ", inv_AP[i]);
            }

            lapack_int info_inv = LAPACKE_stptri(LAPACK_ROW_MAJOR, 'U', 'N', n, inv_AP);

            printf("inv info: %d\n", info_inv);
            show_float_packed_matrix(inv_AP, 'U', n_rows, n_columns, "AP inversed");

            free(inv_AP);

        }
        if(strpbrk(OPS, "s")){

            // RHS
            float *b = NULL;
            create_float_matrix(&b, n, 1);
            auto_fill(&b, n, 1, RANDOM_SEED);
            show_float_matrix(b, n, 1, "b");

            // Ax = b
            float *x = NULL;
            create_float_matrix(&x, n_rows, 1);
            copy_matrix(&x, &b, n_rows);

            lapack_int info_solved_system = LAPACKE_stptrs(LAPACK_ROW_MAJOR, 'U', 'N', 'N', n, 1, AP , x, 1);

            printf("solved system info: %d\n", info_solved_system);
            show_float_matrix(x, n, 1, "x");

            free(x);
            free(b);

        }
    }
    free(ipiv);

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}