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
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <seed>\n", BIN);
        printf("Options to <type>: 's', 'd', 'c', 'z'\n");
        return -5;
    }
    if(strpbrk(MATRIX_TYPE, "gst") == NULL){
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <seed>\n", BIN);
        printf("Options to <matrix_type>: 'g', 's', 't', 'gs', 'gt'\n");
        return -3;
    }
    if(strpbrk(OPS, "fis") == NULL){
        printf("Use: %s <number_rows> <number_columns> <type> <matrix_type> <ops> <seed>\n", BIN);
        printf("Options to <ops>: 'f', 'fi', 'fs', 'fis', 'i', 's'. Obs: to types of matrix g and s, the matrix must be factored to realize others operations\n");
        return -4;
    }

    return 0;
}

int check_args_BLAS(const char *BIN, int A_rows, int A_columns, int B_rows, int B_columns, const char *OP, const int RANDOM_SEED){

    return 0;
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
                (*matrix)[i*rows + j] = rand()%10;
            }else{
                (*matrix)[i*rows + j] = (*matrix)[j*rows + i];
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
                (*matrix)[i*rows + j] = rand()%10;
            }else{
                (*matrix)[i*rows + j] = 0;
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
                (*matrix)[i*rows + j] = rand()%10;
            }else{
                (*matrix)[i*rows + j] = (*matrix)[j*rows + i];
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
                (*matrix)[i*rows + j] = rand()%10;
            }else{
                (*matrix)[i*rows + j] = 0;
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
                (*matrix)[i*rows + j].real = (*matrix)[j*rows + i].real;
                (*matrix)[i*rows + j].imag = (*matrix)[j*rows + i].imag;
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
                (*matrix)[i*rows + j].real = rand()%10;
                (*matrix)[i*rows + j].imag = rand()%10;
            }else{
                (*matrix)[i*rows + j].real = 0;
                (*matrix)[i*rows + j].imag = 0;
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
                (*matrix)[i*rows + j].real = (*matrix)[j*rows + i].real;
                (*matrix)[i*rows + j].imag = (*matrix)[j*rows + i].imag;
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
                (*matrix)[i*rows + j].real = rand()%10;
                (*matrix)[i*rows + j].imag = rand()%10;
            }else{
                (*matrix)[i*rows + j].real = 0;
                (*matrix)[i*rows + j].imag = 0;
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

    cblas_cgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, &alpha, A, A_columns, B, B_rows, &beta, C, B_columns);

    if(SHOW == 's')
        show_complex_float_matrix(C, A_rows, B_columns, "C");
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

    cblas_zgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, A_rows, B_columns, A_columns, &alpha, A, A_columns, B, B_rows, &beta, C, B_columns);

    if(SHOW == 's')
        show_complex_double_matrix(C, A_rows, B_columns, "C");
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
}

// 