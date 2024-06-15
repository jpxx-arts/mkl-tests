#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mkl.h>

int check_args_LAPACK(const char *BIN, int rows, int columns, const char TYPE, const char *MATRIX_TYPE, const char *OPS){
    if(!strcmp(MATRIX_TYPE, "s")){
        if(rows != columns){
            printf("To one symmetric matrix the number of rows and columns must be equal\n");
            return -2;
        }
    }
    if(TYPE != 's' && TYPE != 'd' && TYPE != 'c' && TYPE != 'z'){
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <seed> <specific_test>\n", BIN);
        printf("Options to <type>: 's', 'd', 'c', 'z'\n");
        return -5;
    }
    if(strpbrk(MATRIX_TYPE, "gst") == NULL){
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <seed> <specific_test>\n", BIN);
        printf("Options to <matrix_type>: 'g', 's', 't', 'gs', 'gt'\n");
        return -3;
    }
    if(strpbrk(OPS, "fis") == NULL){
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <seed> <specific_test>\n", BIN);
        printf("Options to <ops>: 'f', 'fi', 'fs', 'fis', 'i', 's'. Obs: to types of matrix g and s, the matrix must be factored to realize others operations\n");
        return -4;
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

void transpose_float_general_matrix(float **matrix, int rows, int columns){
    float *trans = NULL;
    create_float_matrix(&trans, columns, rows);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            trans[i + j*rows] = (*matrix)[i*columns + j];
        }
    }
    
    free(*matrix);
    *matrix = trans;
}

void transpose_double_general_matrix(double **matrix, int rows, int columns){
    double *trans = NULL;
    create_double_matrix(&trans, columns, rows);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            trans[i + j*rows] = (*matrix)[i*columns + j];
        }
    }
    
    free(*matrix);
    *matrix = trans;
}

void transpose_complex_float_general_matrix(lapack_complex_float **matrix, int rows, int columns){
    lapack_complex_float *trans = NULL;
    create_complex_float_matrix(&trans, columns, rows);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            trans[j + i*columns].real = (*matrix)[i + j*columns].real;
            trans[j + i*columns].imag = (*matrix)[i + j*columns].imag;
        }
    }

    free(*matrix);
    *matrix = trans;
}

void transpose_complex_double_general_matrix(lapack_complex_double **matrix, int rows, int columns){
    lapack_complex_double *trans = NULL;
    create_complex_double_matrix(&trans, columns, rows);

    for(int i = 0; i < rows; i++){
        for(int j = 0; j < columns; j++){
            trans[j + i*columns].real = (*matrix)[i + j*columns].real;
            trans[j + i*columns].imag = (*matrix)[i + j*columns].imag;
        }
    }

    free(*matrix);
    *matrix = trans;
}
