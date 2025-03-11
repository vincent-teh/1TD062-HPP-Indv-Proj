#include "generate_mat.h"


Matrix generate_sequential_mat(size_t n)
{
    Matrix mat = {.matrix=(double**) malloc(n * sizeof(double *)),
                  .row=n,
                  .col=n
                 };
    if (mat.matrix == NULL) {
        printf("Memory allocation with size %ld failed!\n", n);
        return mat;
    }

    for (size_t i=0; i<n; i++) {
        mat.matrix[i] = (double *) malloc(n * sizeof(double));
        if (mat.matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (size_t j = 0; j < i; j++) {
                free(mat.matrix[j]);
            }
            free(mat.matrix);
            return mat;
        }
    }

    for (size_t i=0; i<n; i++) {
        for (size_t j=0; j<n; j++) {
            mat.matrix[i][j] = i + j;
        }
    }

    return mat;
}


Matrix generate_zeros_mat(size_t n)
{
    Matrix mat = {.matrix=(double**) malloc(n * sizeof(double *)),
                .row=n,
                .col=n
                };
    if (mat.matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return mat;
    }
    for (size_t i=0; i<n; i++) {
        mat.matrix[i] = (double *) calloc(n, sizeof(double));
        if (mat.matrix[i] == NULL) {
            for (size_t j = 0; j < i; j++) {
                free(mat.matrix[j]);
            }
            free(mat.matrix);
            return mat;
        }
    }
    return mat;
}


void destroy_mat(Matrix mat)
{
    for (size_t i=0; i<mat.col; i++)
        free(mat.matrix[i]);
    free(mat.matrix);
    return;
}
