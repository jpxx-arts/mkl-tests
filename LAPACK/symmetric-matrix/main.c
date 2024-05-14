#include <stdio.h>
#include <stdlib.h>
#include <mkl.h>
#include "symmetric.h"

#define RANDOM_SEED 1

int main(int argc, char const *argv[]){
    double start_time, end_time;
    start_time = dsecnd();

    if(argc != 2) {
        printf("Use: %s <dimension>\n", argv[0]);
        return -1;
    }

    const lapack_int n = atoi(argv[1]);

    // //Fatoração de uma matriz A
    // float *A = (float *) malloc(n*n*sizeof(float));
    // symmetric_fill(&A, n, n, RANDOM_SEED);
    // printf("Matrix A:\n");
    // show_matrix(A, n, n);

    float *AP = (float *) malloc(n*(n+1)/2*sizeof(float));
    packed_symmetric_fill(&AP, n*(n+1)/2, RANDOM_SEED);
    // pack_matrix(&A, &AP, n, n, n);
    // printf("Matrix AP:\n");
    // show_matrix(AP, 1, n*(n+1)/2);

    lapack_int* ipiv = (lapack_int *) malloc(n*sizeof(lapack_int));
    lapack_int info_Bunch_Kaufman = LAPACKE_ssptrf(LAPACK_ROW_MAJOR, 'U', n, AP, ipiv);

    // printf("Bunch_Kaufman info: %d\n\n", info_Bunch_Kaufman);
    
    // //RHS
    float *b = (float *) malloc(n*sizeof(float));
    auto_fill(&b, n, 1, RANDOM_SEED);
    // printf("Matrix b:\n");
    // show_matrix(b, n, 1);

    // //Ax = b
    lapack_int info_solved_system = LAPACKE_ssptrs(LAPACK_ROW_MAJOR, 'U', n, 1, AP, ipiv, b, 1);
    // printf("solved system info: %d\n", info_solved_system);
    printf("Matrix x:\n");
    show_matrix(b, n, 1);

    free(b);
    free(ipiv);
    free(AP);
    // free(A);

    end_time = dsecnd();
    printf("%f s\n", end_time - start_time);

    return 0;
}