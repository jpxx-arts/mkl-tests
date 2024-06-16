#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>

#define ZEROS_PERCENT 0.7

int check_args_BLAS(const char *BIN, int A_rows, int A_columns, int B_rows, int B_columns, const char *ROUTINE, const char TYPE, const char *MATRIX_TYPE){
    if(TYPE != 's' && TYPE != 'd' && TYPE != 'c' && TYPE != 'z'){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <routine> <type> <matrix_type> <show> <seed>\n", BIN);
        printf("Options to <type>: 's', 'd', 'c', 'z'\n");
        return -5;
    }

    if(strcmp(ROUTINE, "vv") && strcmp(ROUTINE, "mv") && strcmp(ROUTINE, "mm")){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <routine> <type> <matrix_type> <show> <seed>\n", BIN);
        printf("Options to <routine>: 'vv', 'mv', 'mm'\n");
        return -6;
    }

    if(strcmp(MATRIX_TYPE, "g") && strcmp(MATRIX_TYPE, "ge") && strcmp(MATRIX_TYPE, "s") && strcmp(MATRIX_TYPE, "ed") && strcmp(MATRIX_TYPE, "ec")){
        printf("Use: %s <A_rows> <A_columns> <B_rows> <B_columns> <routine> <type> <matrix_type> <show> <seed>\n", BIN);
        printf("Options to <matrix_type>: 'g', 'ge', 's', 'ed', 'ec'\n");
        return -7;
    }

    return 0;
}

//Índice K para matriz simétrica
int k_index(int i, int j, int n) {
    return (j - 1) + (i - 1) * (2 * n - i) / 2;
}

// float
int create_float_matrix(float **matrix, int rows, int columns){
    (*matrix) = (float *) malloc(rows*columns * sizeof(float));
    if ((*matrix) == NULL) {
        printf("Allocation error");
        free(*matrix);
        return -1;
    }

    return 0;
}

int create_int_matrix(int **matrix, int rows, int columns){
    (*matrix) = (int *) malloc(rows*columns * sizeof(int));
    if ((*matrix) == NULL) {
        printf("Allocation error");
        free(*matrix);
        return -1;
    }

    return 0;
}

int auto_float_fill(float **matrix, int rows, int columns, int seed){
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

int auto_float_static_fill(float *matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[j + (i*columns)] = rand()%10;
        }
    }

    return 0;
}

int symmetric_float_fill(float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[i*columns + j] = rand()%10;
            }else{
                (*matrix)[i*columns + j] = (*matrix)[j*columns + i];
            }
        }
    }

    return 0;
}

int triangular_float_fill(float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[i*columns + j] = rand()%10;
            }else{
                (*matrix)[i*columns + j] = 0;
            }
        }
    }

    return 0;
}

int show_float_matrix(float *matrix, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("%.2f ", matrix[j + (i*columns)]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

int show_int_matrix(int *matrix, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("%d ", matrix[j + (i*columns)]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

int show_float_packed_matrix(float *matrix, char uplo, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);

    int k = 0;
    switch (uplo){
        case 'U':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i > j){
                        printf("%.2f ", 0.0);
                    }else{
                        printf("%.2f ", matrix[k++]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;

        case 'L':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i <= j){
                        printf("%.2f ", 0.0);
                    }else{
                        printf("%.2f ", matrix[k++]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
        
        // Ajustando para matrizes simétricas
        default:
            for(int i = 1; i <= rows; i++){
                for(int j = 1; j <= columns; j++){
                    if(i <= j){
                        printf("%.2f ", matrix[k_index(i, j, rows)]);
                    }else{
                        printf("%.2f ", matrix[k_index(j, i, rows)]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
    }


    return 0;
}

int pack_float_matrix(float **A, float **AP, int rows, int columns, int n){
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

int packed_float_fill(float **matrix, int size, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < size; i++){
        (*matrix)[i] = rand()%10;
    }

    return 0;
}

int copy_float_matrix(float **dest, float **source, int size){
    if(dest == NULL || source == NULL){
        printf("One or more matrices empty\n");
        return -1;
    }

    for(int i = 0; i < size; i++){
        (*dest)[i] = (*source)[i];
    }

    return 0;
}

// double
int create_double_matrix(double **matrix, int rows, int columns){
    (*matrix) = (double *) malloc(rows*columns * sizeof(double));
    if ((*matrix) == NULL) {
        printf("Allocation error");
        free(*matrix);
        return -1;
    }

    return 0;
}

int auto_double_fill(double **matrix, int rows, int columns, int seed){
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

int auto_double_static_fill(double *matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[j + (i*columns)] = rand()%10;
        }
    }

    return 0;
}

int symmetric_double_fill(double **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[i*columns + j] = rand()%10;
            }else{
                (*matrix)[i*columns + j] = (*matrix)[j*columns + i];
            }
        }
    }

    return 0;
}

int triangular_double_fill(double **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[i*columns + j] = rand()%10;
            }else{
                (*matrix)[i*columns + j] = 0;
            }
        }
    }

    return 0;
}

int show_double_matrix(double *matrix, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("%.2f ", matrix[j + (i*columns)]);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

int show_double_packed_matrix(double *matrix, char uplo, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);

    int k = 0;
    switch (uplo){
        case 'U':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i > j){
                        printf("%.2f ", 0.0);
                    }else{
                        printf("%.2f ", matrix[k++]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;

        case 'L':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i <= j){
                        printf("%.2f ", 0.0);
                    }else{
                        printf("%.2f ", matrix[k++]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
        
        // Ajustando para matrizes simétricas
        default:
            for(int i = 1; i <= rows; i++){
                for(int j = 1; j <= columns; j++){
                    if(i <= j){
                        printf("%.2f ", matrix[k_index(i, j, rows)]);
                    }else{
                        printf("%.2f ", matrix[k_index(j, i, rows)]);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
    }


    return 0;
}

int pack_double_matrix(double **A, double **AP, int rows, int columns, int n){
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

int packed_double_fill(double **matrix, int size, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < size; i++){
        (*matrix)[i] = rand()%10;
    }

    return 0;
}

int copy_double_matrix(double **dest, double **source, int size){
    if(dest == NULL || source == NULL){
        printf("One or more matrices empty\n");
        return -1;
    }

    for(int i = 0; i < size; i++){
        (*dest)[i] = (*source)[i];
    }

    return 0;
}

// Complex float
int create_complex_float_matrix(lapack_complex_float **matrix, int rows, int columns){
    (*matrix) = (lapack_complex_float *) malloc(rows*columns * sizeof(lapack_complex_float));
    if ((*matrix) == NULL) {
        printf("Allocation error");
        free(*matrix);
        return -1;
    }

    return 0;
}

int auto_complex_float_fill(lapack_complex_float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            (*matrix)[j + (i*columns)].real = rand()%10;
            (*matrix)[j + (i*columns)].imag = rand()%10;
        }
    }

    return 0;
}

int auto_complex_float_static_fill(lapack_complex_float *matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[j + (i*columns)].real = rand()%10;
            matrix[j + (i*columns)].imag = rand()%10;
        }
    }

    return 0;
}

int symmetric_complex_float_fill(lapack_complex_float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[j + (i*columns)].real = rand()%10;
                (*matrix)[j + (i*columns)].imag = rand()%10;
            }else{
                (*matrix)[i*columns + j].real = (*matrix)[j*columns + i].real;
                (*matrix)[i*columns + j].imag = (*matrix)[j*columns + i].imag;
            }
        }
    }

    return 0;
}

int triangular_complex_float_fill(lapack_complex_float **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[i*columns + j].real = rand()%10;
                (*matrix)[i*columns + j].imag = rand()%10;
            }else{
                (*matrix)[i*columns + j].real = 0;
                (*matrix)[i*columns + j].imag = 0;
            }
        }
    }

    return 0;
}

int show_complex_float_matrix(lapack_complex_float *matrix, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("(%.2f, %.2f) ", matrix[j + (i*columns)].real, matrix[j + (i*columns)].imag);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

int show_complex_float_packed_matrix(lapack_complex_float *matrix, char uplo, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);

    int k = 0;
    switch (uplo){
        case 'U':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i > j){
                        printf("(%.2f, %.2f) ", 0.0, 0.0);
                    }else{
                        printf("(%.2f, %.2f) ", matrix[k].real, matrix[k].imag);
                        k++;
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;

        case 'L':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i <= j){
                        printf("(%.2f, %.2f) ", 0.0, 0.0);
                    }else{
                        printf("(%.2f, %.2f) ", matrix[k].real, matrix[k].imag);
                        k++;
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
        
        // Ajustando para matrizes simétricas
        default:
            for(int i = 1; i <= rows; i++){
                for(int j = 1; j <= columns; j++){
                    if(i <= j){
                        printf("(%.2f, %.2f) ", matrix[k_index(i, j, rows)].real, matrix[k_index(i, j, rows)].imag);
                    }else{
                        printf("(%.2f, %.2f) ", matrix[k_index(j, i, rows)].real, matrix[k_index(j, i, rows)].imag);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
    }


    return 0;
}

int pack_complex_float_matrix(lapack_complex_float **A, lapack_complex_float **AP, int rows, int columns, int n){
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
    //             (*AP)[k].real = (*A)[i*n + j].real;
    //             (*AP)[k].imag = (*A)[i*n + j].imag;
    //         }
    //     }
    // }

    for(int i = 0, k = 0; i < n; i++){
        for(int j = i; j < n; j++){
            (*AP)[k].real = (*A)[i*n + j].real;
            (*AP)[k].real = (*A)[i*n + j].real;
            k++;
        }
    }

    return 0;
}

int packed_complex_float_fill(lapack_complex_float **matrix, int size, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < size; i++){
        (*matrix)[i].real = rand()%10;
        (*matrix)[i].imag = rand()%10;
    }

    return 0;
}

int copy_complex_float_matrix(lapack_complex_float **dest, lapack_complex_float **source, int size){
    if(dest == NULL || source == NULL){
        printf("One or more matrices empty\n");
        return -1;
    }

    for(int i = 0; i < size; i++){
        (*dest)[i].real = (*source)[i].real;
        (*dest)[i].imag = (*source)[i].imag;
    }

    return 0;
}

// Complex double
int create_complex_double_matrix(lapack_complex_double **matrix, int rows, int columns){
    (*matrix) = (lapack_complex_double *) malloc(rows*columns * sizeof(lapack_complex_double));
    if ((*matrix) == NULL) {
        printf("Allocation error");
        free(*matrix);
        return -1;
    }

    return 0;
}

int auto_complex_double_fill(lapack_complex_double **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            (*matrix)[j + (i*columns)].real = rand()%10;
            (*matrix)[j + (i*columns)].imag = rand()%10;
        }
    }

    return 0;
}

int auto_complex_double_static_fill(lapack_complex_double *matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            matrix[j + (i*columns)].real = rand()%10;
            matrix[j + (i*columns)].imag = rand()%10;
        }
    }

    return 0;
}

int symmetric_complex_double_fill(lapack_complex_double **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[j + (i*columns)].real = rand()%10;
                (*matrix)[j + (i*columns)].imag = rand()%10;
            }else{
                (*matrix)[i*columns + j].real = (*matrix)[j*columns + i].real;
                (*matrix)[i*columns + j].imag = (*matrix)[j*columns + i].imag;
            }
        }
    }

    return 0;
}

int triangular_complex_double_fill(lapack_complex_double **matrix, int rows, int columns, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){    
            if(j >= i){
                (*matrix)[i*columns + j].real = rand()%10;
                (*matrix)[i*columns + j].imag = rand()%10;
            }else{
                (*matrix)[i*columns + j].real = 0;
                (*matrix)[i*columns + j].imag = 0;
            }
        }
    }

    return 0;
}

int show_complex_double_matrix(lapack_complex_double *matrix, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);
    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            printf("(%.2f, %.2f) ", matrix[j + (i*columns)].real, matrix[j + (i*columns)].imag);
        }
        printf("\n");
    }
    printf("\n");

    return 0;
}

int show_complex_double_packed_matrix(lapack_complex_double *matrix, char uplo, int rows, int columns, char *matrix_name){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    printf("Matrix %s:\n", matrix_name);

    int k = 0;
    switch (uplo){
        case 'U':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i > j){
                        printf("(%.2f, %.2f) ", 0.0, 0.0);
                    }else{
                        printf("(%.2f, %.2f) ", matrix[k].real, matrix[k].imag);
                        k++;
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;

        case 'L':
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < columns; j++){
                    if(i <= j){
                        printf("(%.2f, %.2f) ", 0.0, 0.0);
                    }else{
                        printf("(%.2f, %.2f) ", matrix[k].real, matrix[k].imag);
                        k++;
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
        
        // Ajustando para matrizes simétricas
        default:
            for(int i = 1; i <= rows; i++){
                for(int j = 1; j <= columns; j++){
                    if(i <= j){
                        printf("(%.2f, %.2f) ", matrix[k_index(i, j, rows)].real, matrix[k_index(i, j, rows)].imag);
                    }else{
                        printf("(%.2f, %.2f) ", matrix[k_index(j, i, rows)].real, matrix[k_index(j, i, rows)].imag);
                    }
                }
                printf("\n");
            }
            printf("\n");
            break;
    }

    return 0;
}

int pack_complex_double_matrix(lapack_complex_double **A, lapack_complex_double **AP, int rows, int columns, int n){
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
    //             (*AP)[k].real = (*A)[i*n + j].real;
    //             (*AP)[k].imag = (*A)[i*n + j].imag;
    //         }
    //     }
    // }

    for(int i = 0, k = 0; i < n; i++){
        for(int j = i; j < n; j++){
            (*AP)[k].real = (*A)[i*n + j].real;
            (*AP)[k].real = (*A)[i*n + j].real;
            k++;
        }
    }

    return 0;
}

int packed_complex_double_fill(lapack_complex_double **matrix, int size, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < size; i++){
        (*matrix)[i].real = rand()%10;
        (*matrix)[i].imag = rand()%10;
    }

    return 0;
}

int copy_complex_double_matrix(lapack_complex_double **dest, lapack_complex_double **source, int size){
    if(dest == NULL || source == NULL){
        printf("One or more matrices empty\n");
        return -1;
    }

    for(int i = 0; i < size; i++){
        (*dest)[i].real = (*source)[i].real;
        (*dest)[i].imag = (*source)[i].imag;
    }

    return 0;
}

// BLAS
void multiply_float_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const float alpha, const float beta){
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

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, alpha, A, A_columns, B, B_rows, beta, C, B_columns);
    if(SHOW == 's')
        show_float_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_double_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const double alpha, const double beta){
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

    free(C);
    free(B);
    free(A);
}

void multiply_complex_float_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, MKL_Complex8 *alpha, MKL_Complex8 *beta){
    MKL_Complex8 *A = NULL;
    create_complex_float_matrix(&A, A_rows, A_columns);
    
    MKL_Complex8 *B = NULL;
    create_complex_float_matrix(&B, B_rows, B_columns);

    MKL_Complex8 *C = NULL;
    create_complex_float_matrix(&C, A_rows, B_columns);

    auto_complex_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
    if(SHOW == 's')
        show_complex_float_matrix(A, A_rows, A_columns, "A");

    auto_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_float_matrix(B, B_rows, B_columns, "B");

    cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, alpha, A, A_columns, B, B_rows, beta, C, B_columns);

    if(SHOW == 's')
        show_complex_float_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_complex_double_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, MKL_Complex16 *alpha, MKL_Complex16 *beta){
    MKL_Complex16 *A = NULL;
    create_complex_double_matrix(&A, A_rows, A_columns);
    
    MKL_Complex16 *B = NULL;
    create_complex_double_matrix(&B, B_rows, B_columns);

    MKL_Complex16 *C = NULL;
    create_complex_double_matrix(&C, A_rows, B_columns);

    auto_complex_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
    if(SHOW == 's')
        show_complex_double_matrix(A, A_rows, A_columns, "A");

    auto_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_double_matrix(B, B_rows, B_columns, "B");

    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, alpha, A, A_columns, B, B_rows, beta, C, B_columns);

    if(SHOW == 's')
        show_complex_double_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_float_symmetric_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const float alpha, const float beta){
    float *A = NULL;
    create_float_matrix(&A, A_rows, A_columns);
    
    float *B = NULL;
    create_float_matrix(&B, B_rows, B_columns);

    float *C = NULL;
    create_float_matrix(&C, A_rows, B_columns);
    
    symmetric_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
    if(SHOW == 's')
        show_float_matrix(A, A_rows, A_columns, "A");

    auto_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_float_matrix(B, B_rows, B_columns, "B");

    cblas_ssymm(CblasRowMajor, CblasLeft, CblasUpper, A_rows, B_columns, alpha, A, A_rows, B, B_columns, beta, C, B_columns);
    if(SHOW == 's')
        show_float_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_double_symmetric_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const double alpha, const double beta){
    double *A = NULL;
    create_double_matrix(&A, A_rows, A_columns);
    
    double *B = NULL;
    create_double_matrix(&B, B_rows, B_columns);

    double *C = NULL;
    create_double_matrix(&C, A_rows, B_columns);

    symmetric_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
    if(SHOW == 's')
        show_double_matrix(A, A_rows, A_columns, "A");

    auto_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_double_matrix(B, B_rows, B_columns, "B");

    cblas_dsymm(CblasRowMajor, CblasLeft, CblasUpper, A_rows, B_columns, alpha, A, A_rows, B, B_columns, beta, C, B_columns);
    if(SHOW == 's')
        show_double_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_complex_float_symmetric_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, MKL_Complex8 *alpha, MKL_Complex8 *beta){
    MKL_Complex8 *A = NULL;
    create_complex_float_matrix(&A, A_rows, A_columns);
    
    MKL_Complex8 *B = NULL;
    create_complex_float_matrix(&B, B_rows, B_columns);

    MKL_Complex8 *C = NULL;
    create_complex_float_matrix(&C, A_rows, B_columns);

    symmetric_complex_float_fill(&A, A_rows, A_columns, RANDOM_SEED);
    if(SHOW == 's')
        show_complex_float_matrix(A, A_rows, A_columns, "A");

    auto_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_float_matrix(B, B_rows, B_columns, "B");

    cblas_csymm(CblasRowMajor, CblasLeft, CblasUpper, A_rows, B_columns, &alpha, A, A_rows, B, B_columns, &beta, C, B_columns);
    if(SHOW == 's')
        show_complex_float_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_complex_double_symmetric_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, MKL_Complex16 *alpha, MKL_Complex16 *beta){
    MKL_Complex16 *A = NULL;
    create_complex_double_matrix(&A, A_rows, A_columns);
    
    MKL_Complex16 *B = NULL;
    create_complex_double_matrix(&B, B_rows, B_columns);

    MKL_Complex16 *C = NULL;
    create_complex_double_matrix(&C, A_rows, B_columns);

    symmetric_complex_double_fill(&A, A_rows, A_columns, RANDOM_SEED);
    if(SHOW == 's')
        show_complex_double_matrix(A, A_rows, A_columns, "A");

    auto_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_double_matrix(B, B_rows, B_columns, "B");

    cblas_zsymm(CblasRowMajor, CblasLeft, CblasUpper, A_rows, B_columns, &alpha, A, A_rows, B, B_columns, &beta, C, B_columns);
    if(SHOW == 's')
        show_complex_double_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

// Sparse BLAS 
int auto_sparse_float_fill(float **matrix, int rows, int columns, int *non_zeros_number, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(rand() < ZEROS_PERCENT * RAND_MAX){
                (*matrix)[j + (i * columns)] = 0;
            } else {
                (*matrix)[j + (i * columns)] = rand() % 10;
                (*non_zeros_number)++;
            }
        }
    }

    return 0;
}

int auto_sparse_double_fill(double **matrix, int rows, int columns, int *non_zeros_number, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            if(rand() < ZEROS_PERCENT * RAND_MAX){
                (*matrix)[j + (i * columns)] = 0;
            } else {
                (*matrix)[j + (i * columns)] = rand() % 10;
                (*non_zeros_number)++;
            }
        }
    }

    return 0;
}

int auto_sparse_complex_float_fill(lapack_complex_float **matrix, int rows, int columns, int *non_zeros_number, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    *non_zeros_number = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if(rand() < ZEROS_PERCENT * RAND_MAX){
                (*matrix)[j + (i*columns)].real = 0;
                (*matrix)[j + (i*columns)].imag = 0;
            } else{
                (*matrix)[j + (i*columns)].real = rand()%10;
                (*matrix)[j + (i*columns)].imag = rand()%10;
                (*non_zeros_number)++;
            }
        }
    }

    return 0;
}

int auto_sparse_complex_double_fill(lapack_complex_double **matrix, int rows, int columns, int *non_zeros_number, int seed){
    if(matrix == NULL){
        printf("Empty matrix");
        return -1;
    }

    srand(seed);

    *non_zeros_number = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++) {
            if(rand() < ZEROS_PERCENT * RAND_MAX){
                (*matrix)[j + (i*columns)].real = 0;
                (*matrix)[j + (i*columns)].imag = 0;
            } else{
                (*matrix)[j + (i*columns)].real = rand()%10;
                (*matrix)[j + (i*columns)].imag = rand()%10;
                (*non_zeros_number)++;
            }
        }
    }

    return 0;
}

void extract_csr_compression_float_parameters(float *matrix, int rows, int columns, float **values, int **columns_arr, int **pointerB, int **pointerE){
    int non_zeros_number = 0, k = 0;
    for(int i = 0; i < rows; i++){
        (*pointerB)[i] = non_zeros_number;
        for(int j = 0; j < columns; j++){
            if(matrix[j + (i * columns)] != 0){
                non_zeros_number++;
                (*columns_arr)[k] = j;
                (*values)[k++] = matrix[j + (i * columns)];
            }
        }
        (*pointerE)[i] = non_zeros_number;
    }
}

void extract_csr_compression_double_parameters(double *matrix, int rows, int columns, double **values, int **columns_arr, int **pointerB, int **pointerE){
    int non_zeros_number = 0, k = 0;
    for(int i = 0; i < rows; i++){
        (*pointerB)[i] = non_zeros_number;
        for(int j = 0; j < columns; j++){
            if(matrix[j + (i * columns)] != 0){
                non_zeros_number++;
                (*columns_arr)[k] = j;
                (*values)[k++] = matrix[j + (i * columns)];
            }
        }
        (*pointerE)[i] = non_zeros_number;
    }
}

void extract_csr_compression_complex_float_parameters(MKL_Complex8 **matrix, int rows, int columns, MKL_Complex8 **values, int **columns_arr, int **pointerB, int **pointerE){
    int non_zeros_number = 0, k = 0;
    for(int i = 0; i < rows; i++){
        (*pointerB)[i] = non_zeros_number;
        for(int j = 0; j < columns; j++){
            if((*matrix)[j + (i * columns)].real != 0 && (*matrix)[j + (i * columns)].imag != 0){
                non_zeros_number++;
                (*columns_arr)[k] = j;
                (*values)[k].real = (*matrix)[j + (i * columns)].real;
                (*values)[k++].imag = (*matrix)[j + (i * columns)].imag;
            }
        }
        (*pointerE)[i] = non_zeros_number;
    }
}

void extract_csr_compression_complex_double_parameters(MKL_Complex16 **matrix, int rows, int columns, MKL_Complex16 **values, int **columns_arr, int **pointerB, int **pointerE){
    int non_zeros_number = 0, k = 0;
    for(int i = 0; i < rows; i++){
        (*pointerB)[i] = non_zeros_number;
        for(int j = 0; j < columns; j++){
            if((*matrix)[j + (i * columns)].real != 0 && (*matrix)[j + (i * columns)].imag != 0){
                non_zeros_number++;
                (*columns_arr)[k] = j;
                (*values)[k].real = (*matrix)[j + (i * columns)].real;
                (*values)[k++].imag = (*matrix)[j + (i * columns)].imag;
            }
        }
        (*pointerE)[i] = non_zeros_number;
    }
}

void multiply_sparse_float_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const float alpha, const float beta){
    float *A = NULL;
    create_float_matrix(&A, A_rows, A_columns);
    
    float *B = NULL;
    create_float_matrix(&B, B_rows, B_columns);

    float *C = NULL;
    create_float_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_sparse_float_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_float_matrix(A, A_rows, A_columns, "A");

    auto_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_float_matrix(B, B_rows, B_columns, "B");

    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, alpha, A, A_columns, B, B_rows, beta, C, B_columns);
    
    if(SHOW == 's')
        show_float_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_sparse_double_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const double alpha, const double beta){
    double *A = NULL;
    create_double_matrix(&A, A_rows, A_columns);
    
    double *B = NULL;
    create_double_matrix(&B, B_rows, B_columns);

    double *C = NULL;
    create_double_matrix(&C, A_rows, B_columns);

    int non_zeros_number;
    auto_sparse_double_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_double_matrix(A, A_rows, A_columns, "A");

    auto_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_double_matrix(B, B_rows, B_columns, "B");

    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, 1.0, A, A_columns, B, B_rows, 0.0, C, B_columns);
    if(SHOW == 's')
        show_double_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_sparse_complex_float_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, MKL_Complex8 *alpha, MKL_Complex8 *beta){
    MKL_Complex8 *A = NULL;
    create_complex_float_matrix(&A, A_rows, A_columns);
    
    MKL_Complex8 *B = NULL;
    create_complex_float_matrix(&B, B_rows, B_columns);

    MKL_Complex8 *C = NULL;
    create_complex_float_matrix(&C, A_rows, B_columns);

    int non_zeros_number;
    auto_sparse_complex_float_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_complex_float_matrix(A, A_rows, A_columns, "A");

    auto_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_float_matrix(B, B_rows, B_columns, "B");

    cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, alpha, A, A_columns, B, B_rows, beta, C, B_columns);

    if(SHOW == 's')
        show_complex_float_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_sparse_complex_double_general_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, MKL_Complex16 *alpha, MKL_Complex16 *beta){
    MKL_Complex16 *A = NULL;
    create_complex_double_matrix(&A, A_rows, A_columns);
    
    MKL_Complex16 *B = NULL;
    create_complex_double_matrix(&B, B_rows, B_columns);

    MKL_Complex16 *C = NULL;
    create_complex_double_matrix(&C, A_rows, B_columns);

    int non_zeros_number;
    auto_sparse_complex_double_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_complex_double_matrix(A, A_rows, A_columns, "A");

    auto_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_double_matrix(B, B_rows, B_columns, "B");

    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, alpha, A, A_columns, B, B_rows, beta, C, B_columns);

    if(SHOW == 's')
        show_complex_double_matrix(C, A_rows, B_columns, "C");

    free(C);
    free(B);
    free(A);
}

void multiply_sparse_float_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const float alpha, const float beta){
    float *A = NULL;
    create_float_matrix(&A, A_rows, A_columns);
    
    float *B = NULL;
    create_float_matrix(&B, B_rows, B_columns);

    float *C = NULL;
    create_float_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_sparse_float_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);
    
    float *A_csr_values = (float *) malloc(non_zeros_number * sizeof(float));
    int *columns_arr = (int *) malloc(non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    extract_csr_compression_float_parameters(A, A_rows, A_columns, &A_csr_values, &columns_arr, &pointerB, &pointerE);

    if(SHOW == 's')
        show_float_matrix(A, A_rows, A_columns, "A");

    auto_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_float_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_s_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_s_mm(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_float_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(A);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void multiply_sparse_double_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const double alpha, const double beta){
    double *A = NULL;
    create_double_matrix(&A, A_rows, A_columns);
    
    double *B = NULL;
    create_double_matrix(&B, B_rows, B_columns);

    double *C = NULL;
    create_double_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_sparse_double_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);
    
    double *A_csr_values = (double *) malloc(non_zeros_number * sizeof(double));
    int *columns_arr = (int *) malloc(non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    extract_csr_compression_double_parameters(A, A_rows, A_columns, &A_csr_values, &columns_arr, &pointerB, &pointerE);

    if(SHOW == 's')
        show_double_matrix(A, A_rows, A_columns, "A");

    auto_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_double_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_d_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_d_mm(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_double_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(A);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void multiply_sparse_complex_float_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const MKL_Complex8 *alpha, const MKL_Complex8 *beta) {
    MKL_Complex8 *A = NULL;
    create_complex_float_matrix(&A, A_rows, A_columns);
    
    MKL_Complex8 *B = NULL;
    create_complex_float_matrix(&B, B_rows, B_columns);

    MKL_Complex8 *C = NULL;
    create_complex_float_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_sparse_complex_float_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);
    
    MKL_Complex8 *A_csr_values = (MKL_Complex8 *) malloc(non_zeros_number * sizeof(MKL_Complex8));
    int *columns_arr = (int *) malloc(non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    extract_csr_compression_complex_float_parameters(&A, A_rows, A_columns, &A_csr_values, &columns_arr, &pointerB, &pointerE);

    if(SHOW == 's')
        show_complex_float_matrix(A, A_rows, A_columns, "A");

    auto_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_float_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_c_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_c_mm(SPARSE_OPERATION_NON_TRANSPOSE, *alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, *beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_complex_float_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(A);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void multiply_sparse_complex_double_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const MKL_Complex16 *alpha, const MKL_Complex16 *beta) {
    MKL_Complex16 *A = NULL;
    create_complex_double_matrix(&A, A_rows, A_columns);
    
    MKL_Complex16 *B = NULL;
    create_complex_double_matrix(&B, B_rows, B_columns);

    MKL_Complex16 *C = NULL;
    create_complex_double_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_sparse_complex_double_fill(&A, A_rows, A_columns, &non_zeros_number, RANDOM_SEED);
    
    MKL_Complex16 *A_csr_values = (MKL_Complex16 *) malloc(non_zeros_number * sizeof(MKL_Complex16));
    int *columns_arr = (int *) malloc(non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    extract_csr_compression_complex_double_parameters(&A, A_rows, A_columns, &A_csr_values, &columns_arr, &pointerB, &pointerE);

    if(SHOW == 's')
        show_complex_double_matrix(A, A_rows, A_columns, "A");

    auto_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_double_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_z_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_z_mm(SPARSE_OPERATION_NON_TRANSPOSE, *alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, *beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_complex_double_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(A);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void auto_compressed_sparse_float_fill(float *values, int rows, int columns, int *columns_arr, int *pointerB, int *pointerE, int *non_zeros_number, int estimated_size_of_non_zeros_number, int RANDOM_SEED) {
    *non_zeros_number = 0;
    for(int i = 0; i < rows; i++){
        pointerB[i] = *non_zeros_number;
        for(int j = 0; j < columns; j++){
            if(rand() > ZEROS_PERCENT * RAND_MAX){
                if(*non_zeros_number >= estimated_size_of_non_zeros_number){
                    estimated_size_of_non_zeros_number *= 2;
                    values = (float *) realloc(values, estimated_size_of_non_zeros_number * sizeof(float));
                    columns_arr = (int *) realloc(columns_arr, estimated_size_of_non_zeros_number * sizeof(int));

                    if(values == NULL || columns_arr == NULL){
                        printf("Allocation error: realloc\n");
                        return;
                    }
                }
                values[*non_zeros_number] = rand()%10;
                columns_arr[(*non_zeros_number)++] = j;
            }
        }
        pointerE[i] = *non_zeros_number;
    }
}

void auto_compressed_sparse_double_fill(double *values, int rows, int columns, int *columns_arr, int *pointerB, int *pointerE, int *non_zeros_number, int estimated_size_of_non_zeros_number, int RANDOM_SEED) {
    *non_zeros_number = 0;
    for(int i = 0; i < rows; i++){
        pointerB[i] = *non_zeros_number;
        for(int j = 0; j < columns; j++){
            if(rand() > ZEROS_PERCENT * RAND_MAX){
                if(*non_zeros_number >= estimated_size_of_non_zeros_number){
                    estimated_size_of_non_zeros_number *= 2;
                    values = (double *) realloc(values, estimated_size_of_non_zeros_number * sizeof(double));
                    columns_arr = (int *) realloc(columns_arr, estimated_size_of_non_zeros_number * sizeof(int));

                    if(values == NULL || columns_arr == NULL){
                        printf("Allocation error: realloc\n");
                        return;
                    }
                }
                values[*non_zeros_number] = rand()%10;
                columns_arr[(*non_zeros_number)++] = j;
            }
        }
        pointerE[i] = *non_zeros_number;
    }
}

void auto_compressed_sparse_complex_float_fill(MKL_Complex8 *values, int rows, int columns, int *columns_arr, int *pointerB, int *pointerE, int *non_zeros_number, int estimated_size_of_non_zeros_number, int RANDOM_SEED) {
    *non_zeros_number = 0;
    for(int i = 0; i < rows; i++){
        pointerB[i] = *non_zeros_number;
        for(int j = 0; j < columns; j++){
            if(rand() > ZEROS_PERCENT * RAND_MAX){
                if(*non_zeros_number >= estimated_size_of_non_zeros_number){
                    estimated_size_of_non_zeros_number *= 2;
                    values = (MKL_Complex8 *) realloc(values, estimated_size_of_non_zeros_number * sizeof(MKL_Complex8));
                    columns_arr = (int *) realloc(columns_arr, estimated_size_of_non_zeros_number * sizeof(int));

                    if(values == NULL || columns_arr == NULL){
                        printf("Allocation error: realloc\n");
                        return;
                    }
                }
                values[*non_zeros_number].real = rand()%10;
                values[*non_zeros_number].imag = rand()%10;

                columns_arr[(*non_zeros_number)++] = j;
            }
        }
        pointerE[i] = *non_zeros_number;
    }
}

void auto_compressed_sparse_complex_double_fill(MKL_Complex16 *values, int rows, int columns, int *columns_arr, int *pointerB, int *pointerE, int *non_zeros_number, int estimated_size_of_non_zeros_number, int RANDOM_SEED) {
    *non_zeros_number = 0;
    for(int i = 0; i < rows; i++){
        pointerB[i] = *non_zeros_number;
        for(int j = 0; j < columns; j++){
            if(rand() > ZEROS_PERCENT * RAND_MAX){
                if(*non_zeros_number >= estimated_size_of_non_zeros_number){
                    estimated_size_of_non_zeros_number *= 2;
                    values = (MKL_Complex16 *) realloc(values, estimated_size_of_non_zeros_number * sizeof(MKL_Complex16));
                    columns_arr = (int *) realloc(columns_arr, estimated_size_of_non_zeros_number * sizeof(int));

                    if(values == NULL || columns_arr == NULL){
                        printf("Allocation error: realloc\n");
                        return;
                    }
                }
                values[*non_zeros_number].real = rand()%10;
                values[*non_zeros_number].imag = rand()%10;

                columns_arr[(*non_zeros_number)++] = j;
            }
        }
        pointerE[i] = *non_zeros_number;
    }
}

void multiply_compressed_sparse_float_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const float alpha, const float beta){
    int estimated_size_of_non_zeros_number = A_rows*A_columns/2;
    float *A_csr_values = (float *) malloc(estimated_size_of_non_zeros_number * sizeof(float));
    int *columns_arr = (int *) malloc(estimated_size_of_non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    
    float *B = NULL;
    create_float_matrix(&B, B_rows, B_columns);

    float *C = NULL;
    create_float_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_compressed_sparse_float_fill(A_csr_values, A_rows, A_columns, columns_arr, pointerB, pointerE, &non_zeros_number, estimated_size_of_non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_float_matrix(A_csr_values, 1, non_zeros_number, "A compressed");

    auto_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_float_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_s_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_s_mm(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_float_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void multiply_compressed_sparse_double_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const double alpha, const double beta){
    int estimated_size_of_non_zeros_number = A_rows*A_columns/2;
    double *A_csr_values = (double *) malloc(estimated_size_of_non_zeros_number * sizeof(double));
    int *columns_arr = (int *) malloc(estimated_size_of_non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    
    double *B = NULL;
    create_double_matrix(&B, B_rows, B_columns);

    double *C = NULL;
    create_double_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_compressed_sparse_double_fill(A_csr_values, A_rows, A_columns, columns_arr, pointerB, pointerE, &non_zeros_number, estimated_size_of_non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_double_matrix(A_csr_values, 1, non_zeros_number, "A compressed");

    auto_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_double_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_d_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_d_mm(SPARSE_OPERATION_NON_TRANSPOSE, alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_double_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void multiply_compressed_sparse_complex_float_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const MKL_Complex8 *alpha, const MKL_Complex8 *beta){
    int estimated_size_of_non_zeros_number = A_rows*A_columns/2;
    MKL_Complex8 *A_csr_values = (MKL_Complex8 *) malloc(estimated_size_of_non_zeros_number * sizeof(MKL_Complex8));
    int *columns_arr = (int *) malloc(estimated_size_of_non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    
    MKL_Complex8 *B = NULL;
    create_complex_float_matrix(&B, B_rows, B_columns);

    MKL_Complex8 *C = NULL;
    create_complex_float_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_compressed_sparse_complex_float_fill(A_csr_values, A_rows, A_columns, columns_arr, pointerB, pointerE, &non_zeros_number, estimated_size_of_non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_complex_float_matrix(A_csr_values, 1, non_zeros_number, "A compressed");

    auto_complex_float_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_float_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_c_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_c_mm(SPARSE_OPERATION_NON_TRANSPOSE, *alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, *beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_complex_float_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}

void multiply_compressed_sparse_complex_double_matrices(int A_rows, int A_columns, int B_rows, int B_columns, const int RANDOM_SEED, const char SHOW, const MKL_Complex16 *alpha, const MKL_Complex16 *beta){
    int estimated_size_of_non_zeros_number = A_rows*A_columns/2;
    MKL_Complex16 *A_csr_values = (MKL_Complex16 *) malloc(estimated_size_of_non_zeros_number * sizeof(MKL_Complex16));
    int *columns_arr = (int *) malloc(estimated_size_of_non_zeros_number * sizeof(int));
    int *pointerB = (int *) malloc((A_rows) * sizeof(int));
    int *pointerE = (int *) malloc((A_rows) * sizeof(int));
    
    MKL_Complex16 *B = NULL;
    create_complex_double_matrix(&B, B_rows, B_columns);

    MKL_Complex16 *C = NULL;
    create_complex_double_matrix(&C, A_rows, B_columns);
    
    int non_zeros_number;
    auto_compressed_sparse_complex_double_fill(A_csr_values, A_rows, A_columns, columns_arr, pointerB, pointerE, &non_zeros_number, estimated_size_of_non_zeros_number, RANDOM_SEED);

    if(SHOW == 's')
        show_complex_double_matrix(A_csr_values, 1, non_zeros_number, "A compressed");

    auto_complex_double_fill(&B, B_rows, B_columns, RANDOM_SEED + 1);
    if(SHOW == 's')
        show_complex_double_matrix(B, B_rows, B_columns, "B");

    sparse_matrix_t A_csr;
    sparse_status_t status_csr = mkl_sparse_z_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, A_rows, A_columns, pointerB, pointerE, columns_arr, A_csr_values);

    struct matrix_descr descr;
    descr.type = SPARSE_MATRIX_TYPE_GENERAL;
    sparse_status_t info_mult = mkl_sparse_z_mm(SPARSE_OPERATION_NON_TRANSPOSE, *alpha, A_csr, descr, SPARSE_LAYOUT_ROW_MAJOR, B, B_columns, B_columns, *beta, C, B_columns);

    if(SHOW == 's'){
        printf("mult info: %d\n", info_mult);
        show_complex_double_matrix(C, A_rows, B_columns, "C");
    }

    free(C);
    free(B);
    free(pointerE);
    free(pointerB);
    free(columns_arr);
    free(A_csr_values);
    mkl_sparse_destroy(A_csr);
}
