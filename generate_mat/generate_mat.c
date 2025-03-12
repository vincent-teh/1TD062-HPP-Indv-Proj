#include "generate_mat.h"


Matrix generate_sequential_mat(size_t nrow, size_t ncol)
{
    Matrix mat = {.matrix=(double**) malloc(nrow * sizeof(double *)),
                  .row=nrow,
                  .col=ncol
                 };
    if (mat.matrix == NULL) {
        printf("Memory allocation with size %ld failed!\n", nrow);
        return mat;
    }

    for (size_t i=0; i<nrow; i++) {
        mat.matrix[i] = (double *) malloc(ncol * sizeof(double));
        if (mat.matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (size_t j = 0; j < i; j++) {
                free(mat.matrix[j]);
            }
            free(mat.matrix);
            return mat;
        }
    }

    for (size_t i=0; i<nrow; i++) {
        for (size_t j=0; j<ncol; j++) {
            mat.matrix[i][j] = (i + j) % 100;
        }
    }

    return mat;
}


Matrix generate_zeros_mat(size_t nrow, size_t ncol)
{
    Matrix mat = {.matrix=(double**) malloc(nrow * sizeof(double *)),
                .row=nrow,
                .col=ncol
                };
    if (mat.matrix == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return mat;
    }
    for (size_t i=0; i<nrow; i++) {
        mat.matrix[i] = (double *) calloc(ncol, sizeof(double));
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

Matrix generate_random_mat(size_t nrow, size_t ncol)
{
    Matrix mat = {
        .matrix = (double**) malloc(nrow * sizeof(double*)),
        .row = nrow,
        .col = ncol
    };

    if (mat.matrix == NULL) {
        printf("Memory allocation with size %ld failed!\n", nrow);
        return mat;
    }

    srand(SEED);

    for (size_t i = 0; i < nrow; i++) {
        mat.matrix[i] = (double*) malloc(ncol * sizeof(double));
        if (mat.matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (size_t j = 0; j < i; j++) {
                free(mat.matrix[j]);
            }
            free(mat.matrix);
            return mat;
        }
    }

    for (size_t i = 0; i < nrow; i++) {
        for (size_t j = 0; j < ncol; j++) {
            mat.matrix[i][j] = (double) rand() / RAND_MAX; // Random number between 0 and 1
        }
    }

    return mat;
}

void destroy_mat(Matrix mat)
{
    for (size_t i=0; i<mat.row; i++)
        free(mat.matrix[i]);
    free(mat.matrix);
    return;
}
