#include <stdio.h>
#include <stdlib.h>

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
            (*matrix)[j + (i*columns)] = rand();
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
            if(j >= i){
                (*matrix)[i*rows + j] = rand();
            }else{
                (*matrix)[i*rows + j] = (*matrix)[j*rows + i];
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
    // for(int i = 0; i < rows; i++){
    //     for(int j = 0; j < columns; j++){
    //         if(i <= j){
    //             k = j + i*(2*n - (i+1))/2;
    //             (*AP)[k] = (*A)[i*n + j];
    //         }
    //     }
    // }

    for(int i = 0, k = 0; i < n; i++){
        for(int j = i; j < n; j++){
            (*AP)[k] = (*A)[i*n + j];
            k++;
        }
    }

    return 0;
}

int packed_symmetric_fill(float **matrix, int size, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < size; i++){
        (*matrix)[i] = rand();
    }

    return 0;
}