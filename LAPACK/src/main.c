#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>
#include "../include/alg_lin.h"

int main(int argc, const char *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 9) {
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <show> <seed> <specific_test>\n", argv[0]);
        return -1;
    }

    // Extraindo argumentos da execução
    lapack_int n_rows = atoi(argv[1]), n_columns = atoi(argv[2]);
    const char TYPE = argv[3][0], *MATRIX_TYPE = argv[4], *OPS = argv[5], SHOW = argv[6][0], RANDOM_SEED = atoi(argv[7]), SPECIFIC_TEST = argv[8][0];

    // Validando argumentos
    int err_code = check_args_LAPACK(argv[0], n_rows, n_columns, TYPE, MATRIX_TYPE, OPS);
    if(err_code)
        return err_code;

    lapack_int *ipiv = NULL;
    int n = n_rows;

    // Fluxo de tipo da matriz:
    switch (TYPE){
        // float
        case 's': 
            // Para matrizes gerais
            if(!strcmp(MATRIX_TYPE, "g")){

                // Criação de uma matriz A
                float *A = NULL;
                create_float_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                auto_float_fill(&A, n_rows, n_columns, RANDOM_SEED);

                if(SHOW == 's')
                    show_float_matrix(A, n_rows, n_columns, "A");

                if(SPECIFIC_TEST == 't'){
                    transpose_float_general_matrix(&A, n_rows, n_columns);
                    if(SHOW == 's')
                        show_float_matrix(A, n_columns, n_rows, "A transposed");
                }

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_float_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

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
                    copy_float_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_float_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

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
                    auto_float_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_float_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    float *x = NULL;
                    create_float_matrix(&x, n_rows, 1);
                    copy_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system;
                    switch(SPECIFIC_TEST){
                        case 'T': {
                            info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'T', n_rows, 1, A, n_columns, ipiv, x, 1);

                            break;
                        }
                        default: {
                            info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                        }
                    }
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_float_matrix(x, n_rows, 1, "x");
                        if(SPECIFIC_TEST == 't' || SPECIFIC_TEST == 'T')
                            printf("*A was transposed*\n");
                    }

                    free(x);
                    free(b);
                }
            }

            // General symmmetric matrix
            else if(!strcmp(MATRIX_TYPE, "gs")){

                // Criação de uma matriz A
                float *A = NULL;
                create_float_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                symmetric_float_fill(&A, n_rows, n_columns, RANDOM_SEED);

                if(SHOW == 's')
                    show_float_matrix(A, n_rows, n_columns, "A");

                if(SPECIFIC_TEST == 't'){
                    transpose_float_general_matrix(&A, n_rows, n_columns);
                    if(SHOW == 's')
                        show_float_matrix(A, n_columns, n_rows, "A transposed");
                }

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_float_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

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
                    copy_float_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_float_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

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
                    auto_float_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_float_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    float *x = NULL;
                    create_float_matrix(&x, n_rows, 1);
                    copy_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system;
                    switch(SPECIFIC_TEST){
                        case 'T': {
                            info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'T', n_rows, 1, A, n_columns, ipiv, x, 1);

                            break;
                        }
                        default: {
                            info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                        }
                    }
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_float_matrix(x, n_rows, 1, "x");
                        if(SPECIFIC_TEST == 't' || SPECIFIC_TEST == 'T')
                            printf("*A was transposed*\n");
                    }

                    free(x);
                    free(b);
                }
            }

            else if(!strcmp(MATRIX_TYPE, "gt")){

                // Criação de uma matriz A
                float *A = NULL;
                create_float_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                triangular_float_fill(&A, n_rows, n_columns, RANDOM_SEED);

                if(SHOW == 's')
                    show_float_matrix(A, n_rows, n_columns, "A");

                if(SPECIFIC_TEST == 't'){
                    transpose_float_general_matrix(&A, n_rows, n_columns);
                    if(SHOW == 's')
                        show_float_matrix(A, n_columns, n_rows, "A transposed");
                }

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_sgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_float_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

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
                    copy_float_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_sgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_float_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

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
                    auto_float_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_float_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    float *x = NULL;
                    create_float_matrix(&x, n_rows, 1);
                    copy_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system;
                    switch(SPECIFIC_TEST){
                        case 'T': {
                            info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'T', n_rows, 1, A, n_columns, ipiv, x, 1);

                            break;
                        }
                        default: {
                            info_solved_system = LAPACKE_sgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                        }
                    }
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_float_matrix(x, n_rows, 1, "x");
                        if(SPECIFIC_TEST == 't' || SPECIFIC_TEST == 'T')
                            printf("*A was transposed*\n");
                    }

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
                packed_float_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_float_packed_matrix(AP, 'N', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz AP simétrica
                    ipiv = (lapack_int *) malloc(n*sizeof(lapack_int));
                    lapack_int info_Bunch_Kaufman = LAPACKE_ssptrf(LAPACK_ROW_MAJOR, 'U', n, AP, ipiv);
                    
                    if(SHOW == 's'){
                        printf("Bunch-Kaufman info: %d\n", info_Bunch_Kaufman);
                        show_float_matrix(AP, 1, size, "AP factorized");
                    }

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
                    copy_float_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_ssptri(LAPACK_ROW_MAJOR, 'U', n, inv_AP, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_float_packed_matrix(inv_AP, 'N', n_rows, n_columns, "AP inversed");
                    }

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
                    auto_float_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_float_matrix(b, n, 1, "b");

                    // Ax = b
                    float *x = NULL;
                    create_float_matrix(&x, n_rows, 1);
                    copy_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_ssptrs(LAPACK_ROW_MAJOR, 'U', n, 1, AP, ipiv , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_float_matrix(x, n, 1, "x");
                    }

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
                packed_float_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_float_packed_matrix(AP, 'U', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "i")){

                    // Inversão de uma matriz A
                    float *inv_AP = NULL;
                    create_float_matrix(&inv_AP, 1, size);
                    copy_float_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_stptri(LAPACK_ROW_MAJOR, 'U', 'N', n, inv_AP);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_float_packed_matrix(inv_AP, 'U', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){

                    // RHS
                    float *b = NULL;
                    create_float_matrix(&b, n, 1);
                    auto_float_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_float_matrix(b, n, 1, "b");

                    // Ax = b
                    float *x = NULL;
                    create_float_matrix(&x, n_rows, 1);
                    copy_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_stptrs(LAPACK_ROW_MAJOR, 'U', 'N', 'N', n, 1, AP , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_float_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }
            break;

        // double
        case 'd':
            // Para matrizes gerais
            if(!strcmp(MATRIX_TYPE, "g")){

                // Criação de uma matriz A
                double *A = NULL;
                create_double_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                auto_double_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_double_matrix(A, n_rows, n_columns, "A");

                if(SPECIFIC_TEST == 't'){
                    transpose_double_general_matrix(&A, n_rows, n_columns);
                    if(SHOW == 's'){
                        show_double_matrix(A, n_columns, n_rows, "A transposed");
                    }
                }

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_double_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // Inversão de uma matriz A
                    double *inv_A = NULL;
                    create_double_matrix(&inv_A, n_rows, n_columns);
                    copy_double_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_dgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_double_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // RHS
                    double *b = NULL;
                    create_double_matrix(&b, n_rows, 1);
                    auto_double_fill(&b, n_rows, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_double_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    double *x = NULL;
                    create_double_matrix(&x, n_rows, 1);
                    copy_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system;
                    switch(SPECIFIC_TEST){
                        case 'T': {
                            info_solved_system = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'T', n_rows, 1, A, n_columns, ipiv, x, 1);

                            break;
                        }
                        default: {
                            info_solved_system = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                        }
                    }
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_double_matrix(x, n_rows, 1, "x");
                        if(SPECIFIC_TEST == 't' || SPECIFIC_TEST == 'T')
                            printf("*A was transposed*\n");
                    }

                    free(x);
                    free(b);
                }
            }

            // General symmmetric matrix
            else if(!strcmp(MATRIX_TYPE, "gs")){

                // Criação de uma matriz A
                double *A = NULL;
                create_double_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                symmetric_double_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_double_matrix(A, n_rows, n_columns, "A");

                if(SPECIFIC_TEST == 't'){
                    transpose_double_general_matrix(&A, n_rows, n_columns);
                    if(SHOW == 's'){
                        show_double_matrix(A, n_columns, n_rows, "A transposed");
                    }
                }

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_double_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // Inversão de uma matriz A
                    double *inv_A = NULL;
                    create_double_matrix(&inv_A, n_rows, n_columns);
                    copy_double_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_dgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_double_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // RHS
                    double *b = NULL;
                    create_double_matrix(&b, n_rows, 1);
                    auto_double_fill(&b, n_rows, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_double_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    double *x = NULL;
                    create_double_matrix(&x, n_rows, 1);
                    copy_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system;
                    switch(SPECIFIC_TEST){
                        case 'T': {
                            info_solved_system = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'T', n_rows, 1, A, n_columns, ipiv, x, 1);

                            break;
                        }
                        default: {
                            info_solved_system = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                        }
                    }
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_double_matrix(x, n_rows, 1, "x");
                        if(SPECIFIC_TEST == 't' || SPECIFIC_TEST == 'T')
                            printf("*A was transposed*\n");
                    }

                    free(x);
                    free(b);
                }
            }

            else if(!strcmp(MATRIX_TYPE, "gt")){

                // Criação de uma matriz A
                double *A = NULL;
                create_double_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                triangular_double_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_double_matrix(A, n_rows, n_columns, "A");

                if(SPECIFIC_TEST == 't'){
                    transpose_double_general_matrix(&A, n_rows, n_columns);
                    if(SHOW == 's'){
                        show_double_matrix(A, n_columns, n_rows, "A transposed");
                    }
                }

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_dgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_double_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // Inversão de uma matriz A
                    double *inv_A = NULL;
                    create_double_matrix(&inv_A, n_rows, n_columns);
                    copy_double_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_dgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_double_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // RHS
                    double *b = NULL;
                    create_double_matrix(&b, n_rows, 1);
                    auto_double_fill(&b, n_rows, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_double_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    double *x = NULL;
                    create_double_matrix(&x, n_rows, 1);
                    copy_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system;
                    switch(SPECIFIC_TEST){
                        case 'T': {
                            info_solved_system = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'T', n_rows, 1, A, n_columns, ipiv, x, 1);

                            break;
                        }
                        default: {
                            info_solved_system = LAPACKE_dgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                        }
                    }
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_double_matrix(x, n_rows, 1, "x");
                        if(SPECIFIC_TEST == 't' || SPECIFIC_TEST == 'T')
                            printf("*A was transposed*\n");
                    }

                    free(x);
                    free(b);
                }
            }

            // Para matrizes simétricas empacotadas
            else if(!strcmp(MATRIX_TYPE, "s")){

                // Criação de uma matriz AP
                double *AP = NULL;
                int size = n*(n+1)/2;
                create_double_matrix(&AP, 1, size);

                // Preenchimento de uma packed matrix
                packed_double_fill(&AP, size, RANDOM_SEED);

                if(SHOW == 's')            
                    show_double_packed_matrix(AP, 'N', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz AP simétrica
                    ipiv = (lapack_int *) malloc(n*sizeof(lapack_int));
                    lapack_int info_Bunch_Kaufman = LAPACKE_dsptrf(LAPACK_ROW_MAJOR, 'U', n, AP, ipiv);
                    
                    if(SHOW == 's'){
                        printf("Bunch-Kaufman info: %d\n", info_Bunch_Kaufman);
                        show_double_matrix(AP, 1, size, "AP factorized");
                    }

                    if(!strcmp(OPS, "f"))
                        free(AP);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                            
                        return -4;
                    }

                    // Inversão de uma matriz A
                    double *inv_AP = NULL;
                    create_double_matrix(&inv_AP, 1, size);
                    copy_double_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_dsptri(LAPACK_ROW_MAJOR, 'U', n, inv_AP, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_double_packed_matrix(inv_AP, 'N', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // RHS
                    double *b = NULL;
                    create_double_matrix(&b, n, 1);
                    auto_double_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_double_matrix(b, n, 1, "b");

                    // Ax = b
                    double *x = NULL;
                    create_double_matrix(&x, n_rows, 1);
                    copy_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_dsptrs(LAPACK_ROW_MAJOR, 'U', n, 1, AP, ipiv , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_double_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }

            // Para matrizes tringulares empacotadas
            else if(!strcmp(MATRIX_TYPE, "t")){

                // Criação de uma matriz AP
                double *AP = NULL;
                int size = n*(n+1)/2;
                create_double_matrix(&AP, 1, size);

                // Preenchimento de uma packed matrix
                packed_double_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_double_packed_matrix(AP, 'U', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "i")){

                    // Inversão de uma matriz A
                    double *inv_AP = NULL;
                    create_double_matrix(&inv_AP, 1, size);
                    copy_double_matrix(&inv_AP, &AP, size);



                    lapack_int info_inv = LAPACKE_dtptri(LAPACK_ROW_MAJOR, 'U', 'N', n, inv_AP);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_double_packed_matrix(inv_AP, 'U', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){

                    // RHS
                    double *b = NULL;
                    create_double_matrix(&b, n, 1);
                    auto_double_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_double_matrix(b, n, 1, "b");

                    // Ax = b
                    double *x = NULL;
                    create_double_matrix(&x, n_rows, 1);
                    copy_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_dtptrs(LAPACK_ROW_MAJOR, 'U', 'N', 'N', n, 1, AP , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_double_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }
            break;

        // complex float
        case 'c':
            // Para matrizes gerais
            if(!strcmp(MATRIX_TYPE, "g")){

                // Criação de uma matriz A
                lapack_complex_float *A = NULL;
                create_complex_float_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                auto_complex_float_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_float_matrix(A, n_rows, n_columns, "A");

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_cgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_complex_float_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_float *inv_A = NULL;
                    create_complex_float_matrix(&inv_A, n_rows, n_columns);
                    copy_complex_float_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_cgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_float_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // RHS
                    lapack_complex_float *b = NULL;
                    create_complex_float_matrix(&b, n_rows, 1);
                    auto_complex_float_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_complex_float_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    lapack_complex_float *x = NULL;
                    create_complex_float_matrix(&x, n_rows, 1);
                    copy_complex_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_cgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_float_matrix(x, n_rows, 1, "x");
                    }

                    free(x);
                    free(b);
                }
            }

            // General symmmetric matrix
            else if(!strcmp(MATRIX_TYPE, "gs")){

                // Criação de uma matriz A
                lapack_complex_float *A = NULL;
                create_complex_float_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                symmetric_complex_float_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_float_matrix(A, n_rows, n_columns, "A");

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_cgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_complex_float_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_float *inv_A = NULL;
                    create_complex_float_matrix(&inv_A, n_rows, n_columns);
                    copy_complex_float_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_cgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_float_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // RHS
                    lapack_complex_float *b = NULL;
                    create_complex_float_matrix(&b, n_rows, 1);
                    auto_complex_float_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_complex_float_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    lapack_complex_float *x = NULL;
                    create_complex_float_matrix(&x, n_rows, 1);
                    copy_complex_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_cgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_float_matrix(x, n_rows, 1, "x");
                    }

                    free(x);
                    free(b);
                }
            }

            else if(!strcmp(MATRIX_TYPE, "gt")){

                // Criação de uma matriz A
                lapack_complex_float *A = NULL;
                create_complex_float_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                triangular_complex_float_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_float_matrix(A, n_rows, n_columns, "A");

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_cgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_complex_float_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_float *inv_A = NULL;
                    create_complex_float_matrix(&inv_A, n_rows, n_columns);
                    copy_complex_float_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_cgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_float_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // RHS
                    lapack_complex_float *b = NULL;
                    create_complex_float_matrix(&b, n_rows, 1);
                    auto_complex_float_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_complex_float_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    lapack_complex_float *x = NULL;
                    create_complex_float_matrix(&x, n_rows, 1);
                    copy_complex_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_cgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_float_matrix(x, n_rows, 1, "x");
                    }

                    free(x);
                    free(b);
                }
            }

            // Para matrizes simétricas empacotadas
            else if(!strcmp(MATRIX_TYPE, "s")){

                // Criação de uma matriz AP
                lapack_complex_float *AP = NULL;
                int size = n*(n+1)/2;
                create_complex_float_matrix(&AP, 1, size);

                // Preenchimento de uma packed matrix
                packed_complex_float_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_float_packed_matrix(AP, 'N', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz AP simétrica
                    ipiv = (lapack_int *) malloc(n*sizeof(lapack_int));
                    lapack_int info_Bunch_Kaufman = LAPACKE_csptrf(LAPACK_ROW_MAJOR, 'U', n, AP, ipiv);
                    
                    if(SHOW == 's'){
                        printf("Bunch-Kaufman info: %d\n", info_Bunch_Kaufman);
                        show_complex_float_matrix(AP, 1, size, "AP factorized");
                    }

                    if(!strcmp(OPS, "f"))
                        free(AP);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_float *inv_AP = NULL;
                    create_complex_float_matrix(&inv_AP, 1, size);
                    copy_complex_float_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_csptri(LAPACK_ROW_MAJOR, 'U', n, inv_AP, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_float_packed_matrix(inv_AP, 'N', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // RHS
                    lapack_complex_float *b = NULL;
                    create_complex_float_matrix(&b, n, 1);
                    auto_complex_float_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_complex_float_matrix(b, n, 1, "b");

                    // Ax = b
                    lapack_complex_float *x = NULL;
                    create_complex_float_matrix(&x, n_rows, 1);
                    copy_complex_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_csptrs(LAPACK_ROW_MAJOR, 'U', n, 1, AP, ipiv , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_float_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }

            // Para matrizes tringulares empacotadas
            else if(!strcmp(MATRIX_TYPE, "t")){

                // Criação de uma matriz AP
                lapack_complex_float *AP = NULL;
                int size = n*(n+1)/2;
                create_complex_float_matrix(&AP, 1, size);

                // Preenchimento de uma packed matrix
                packed_complex_float_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_float_packed_matrix(AP, 'U', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "i")){

                    // Inversão de uma matriz A
                    lapack_complex_float *inv_AP = NULL;
                    create_complex_float_matrix(&inv_AP, 1, size);
                    copy_complex_float_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_ctptri(LAPACK_ROW_MAJOR, 'U', 'N', n, inv_AP);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_float_packed_matrix(inv_AP, 'U', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){

                    // RHS
                    lapack_complex_float *b = NULL;
                    create_complex_float_matrix(&b, n, 1);
                    auto_complex_float_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_complex_float_matrix(b, n, 1, "b");

                    // Ax = b
                    lapack_complex_float *x = NULL;
                    create_complex_float_matrix(&x, n_rows, 1);
                    copy_complex_float_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_ctptrs(LAPACK_ROW_MAJOR, 'U', 'N', 'N', n, 1, AP , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_float_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }
            break;

        // complex double
        case 'z':
            // Para matrizes gerais
            if(!strcmp(MATRIX_TYPE, "g")){

                // Criação de uma matriz A
                lapack_complex_double *A = NULL;
                create_complex_double_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                auto_complex_double_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_double_matrix(A, n_rows, n_columns, "A");

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_zgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_complex_double_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_double *inv_A = NULL;
                    create_complex_double_matrix(&inv_A, n_rows, n_columns);
                    copy_complex_double_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_zgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_double_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // RHS
                    lapack_complex_double *b = NULL;
                    create_complex_double_matrix(&b, n_rows, 1);
                    auto_complex_double_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_complex_double_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    lapack_complex_double *x = NULL;
                    create_complex_double_matrix(&x, n_rows, 1);
                    copy_complex_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_zgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_double_matrix(x, n_rows, 1, "x");
                    }

                    free(x);
                    free(b);
                }
            }

            // General symmmetric matrix
            else if(!strcmp(MATRIX_TYPE, "gs")){

                // Criação de uma matriz A
                lapack_complex_double *A = NULL;
                create_complex_double_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                symmetric_complex_double_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_double_matrix(A, n_rows, n_columns, "A");

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_zgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_complex_double_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_double *inv_A = NULL;
                    create_complex_double_matrix(&inv_A, n_rows, n_columns);
                    copy_complex_double_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_zgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_double_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // RHS
                    lapack_complex_double *b = NULL;
                    create_complex_double_matrix(&b, n_rows, 1);
                    auto_complex_double_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_complex_double_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    lapack_complex_double *x = NULL;
                    create_complex_double_matrix(&x, n_rows, 1);
                    copy_complex_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_zgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_double_matrix(x, n_rows, 1, "x");
                    }

                    free(x);
                    free(b);
                }
            }

            else if(!strcmp(MATRIX_TYPE, "g")){

                // Criação de uma matriz A
                lapack_complex_double *A = NULL;
                create_complex_double_matrix(&A, n_rows, n_columns);

                // Preenchimento de uma matriz geral
                triangular_complex_double_fill(&A, n_rows, n_columns, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_double_matrix(A, n_rows, n_columns, "A");

                // Fluxo de operações com a matriz geral
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz A geral
                    ipiv = (lapack_int *) malloc(n_columns*sizeof(lapack_int));
                    lapack_int info_LU = LAPACKE_zgetrf(LAPACK_ROW_MAJOR, n_rows, n_columns, A, n_columns, ipiv);
                    
                    if(SHOW == 's'){
                        printf("LU info: %d\n", info_LU);
                        show_complex_double_matrix(A, n_rows, n_columns, "A factorized");

                        show_int_matrix(ipiv, 1, n_columns, "ipiv");
                    }

                    if(!strcmp(OPS, "f"))
                        free(A);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_double *inv_A = NULL;
                    create_complex_double_matrix(&inv_A, n_rows, n_columns);
                    copy_complex_double_matrix(&inv_A, &A, n_rows*n_columns);

                    lapack_int info_inv = LAPACKE_zgetri(LAPACK_ROW_MAJOR, n_rows, inv_A, n_columns, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_double_matrix(inv_A, n_rows, n_columns, "A inversed");
                    }

                    free(inv_A);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");
                        
                        return -4;
                    }

                    // RHS
                    lapack_complex_double *b = NULL;
                    create_complex_double_matrix(&b, n_rows, 1);
                    auto_complex_double_fill(&b, n_rows, 1, RANDOM_SEED + 1);
                    
                    if(SHOW == 's')
                        show_complex_double_matrix(b, n_rows, 1, "b");

                    // Ax = b
                    lapack_complex_double *x = NULL;
                    create_complex_double_matrix(&x, n_rows, 1);
                    copy_complex_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_zgetrs(LAPACK_ROW_MAJOR, 'N', n_rows, 1, A, n_columns, ipiv, x, 1);
                    
                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_double_matrix(x, n_rows, 1, "x");
                    }

                    free(x);
                    free(b);
                }
            }

            // Para matrizes simétricas empacotadas
            else if(!strcmp(MATRIX_TYPE, "s")){

                // Criação de uma matriz AP
                lapack_complex_double *AP = NULL;
                int size = n*(n+1)/2;
                create_complex_double_matrix(&AP, 1, size);

                // Preenchimento de uma packed matrix
                packed_complex_double_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_double_packed_matrix(AP, 'N', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "f")){

                    // Fatoração de uma matriz AP simétrica
                    ipiv = (lapack_int *) malloc(n*sizeof(lapack_int));
                    lapack_int info_Bunch_Kaufman = LAPACKE_zsptrf(LAPACK_ROW_MAJOR, 'U', n, AP, ipiv);
                    
                    if(SHOW == 's'){
                        printf("Bunch-Kaufman info: %d\n", info_Bunch_Kaufman);
                        show_complex_double_matrix(AP, 1, size, "AP factorized");
                    }

                    if(!strcmp(OPS, "f"))
                        free(AP);

                }
                if(strpbrk(OPS, "i")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // Inversão de uma matriz A
                    lapack_complex_double *inv_AP = NULL;
                    create_complex_double_matrix(&inv_AP, 1, size);
                    copy_complex_double_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_zsptri(LAPACK_ROW_MAJOR, 'U', n, inv_AP, ipiv);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_double_packed_matrix(inv_AP, 'N', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){
                    if(!strpbrk(OPS, "f")){
                        printf("To do this operation the matrix must be factorized\n");

                        return -4;
                    }

                    // RHS
                    lapack_complex_double *b = NULL;
                    create_complex_double_matrix(&b, n, 1);
                    auto_complex_double_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_complex_double_matrix(b, n, 1, "b");

                    // Ax = b
                    lapack_complex_double *x = NULL;
                    create_complex_double_matrix(&x, n_rows, 1);
                    copy_complex_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_zsptrs(LAPACK_ROW_MAJOR, 'U', n, 1, AP, ipiv , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_double_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }

            // Para matrizes tringulares empacotadas
            else if(!strcmp(MATRIX_TYPE, "t")){

                // Criação de uma matriz AP
                lapack_complex_double *AP = NULL;
                int size = n*(n+1)/2;
                create_complex_double_matrix(&AP, 1, size);

                // Preenchimento de uma packed matrix
                packed_complex_double_fill(&AP, size, RANDOM_SEED);
                
                if(SHOW == 's')
                    show_complex_double_packed_matrix(AP, 'U', n_rows, n_columns, "AP");
            
                // Fluxo de operações com a matriz simétrica
                if(strpbrk(OPS, "i")){

                    // Inversão de uma matriz A
                    lapack_complex_double *inv_AP = NULL;
                    create_complex_double_matrix(&inv_AP, 1, size);
                    copy_complex_double_matrix(&inv_AP, &AP, size);

                    lapack_int info_inv = LAPACKE_ztptri(LAPACK_ROW_MAJOR, 'U', 'N', n, inv_AP);

                    if(SHOW == 's'){
                        printf("inv info: %d\n", info_inv);
                        show_complex_double_packed_matrix(inv_AP, 'U', n_rows, n_columns, "AP inversed");
                    }

                    free(inv_AP);

                }
                if(strpbrk(OPS, "s")){

                    // RHS
                    lapack_complex_double *b = NULL;
                    create_complex_double_matrix(&b, n, 1);
                    auto_complex_double_fill(&b, n, 1, RANDOM_SEED + 1);

                    if(SHOW == 's')
                        show_complex_double_matrix(b, n, 1, "b");

                    // Ax = b
                    lapack_complex_double *x = NULL;
                    create_complex_double_matrix(&x, n_rows, 1);
                    copy_complex_double_matrix(&x, &b, n_rows);

                    lapack_int info_solved_system = LAPACKE_ztptrs(LAPACK_ROW_MAJOR, 'U', 'N', 'N', n, 1, AP , x, 1);

                    if(SHOW == 's'){
                        printf("solved system info: %d\n", info_solved_system);
                        show_complex_double_matrix(x, n, 1, "x");
                    }

                    free(x);
                    free(b);

                }
            }
            break;
    }

    free(ipiv);

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}
