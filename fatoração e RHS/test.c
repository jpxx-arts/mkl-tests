#include <stdio.h>
#include <mkl.h>

int main() {
    const int N = 4;
    double A[N][N] = {
        {1.0, 2.0, 3.0, 4.0},
        {0.0, 5.0, 6.0, 7.0},
        {0.0, 0.0, 8.0, 9.0},
        {0.0, 0.0, 0.0, 10.0}
    };
    double AP[N * (N + 1) / 2]; // Upper packed matrix

    // Convertendo a matriz original para a forma compactada
    char uplo = 'U'; // Upper triangular
    char diag = 'N'; // Non-unit diagonal
    for (int i = 0, k = 0; i < N; i++) {
        for (int j = i; j < N; j++) {
            AP[k++] = A[i][j];
        }
    }

    // Agora você pode usar a matriz compactada (AP) com as funções apropriadas do MKL

    // Por exemplo, calcular o produto de uma matriz compactada por um vetor
    double x[N] = {1.0, 2.0, 3.0, 4.0};
    double y[N];

    cblas_dspmv(CblasRowMajor, CblasUpper, N, 1.0, AP, x, 1, 0.0, y, 1);

    // Exibir o resultado
    printf("Resultado do produto AP*x:\n");
    for (int i = 0; i < N; i++) {
        printf("%.2f\n", y[i]);
    }

    return 0;
}
