#ifndef H_GENERATE_MAT_H
#define H_GENERATE_MAT_H

#include <stdio.h>
#include <stdlib.h>

#define SEED 49

typedef struct
{
    double** matrix;
    size_t row;
    size_t col;
} Matrix;
 

/**
 * @brief Generate sequential matrix of size n*n.
 * 
 * @param n size of the matrix.
 * @return double** Pointer to the matrix. Null if allocation error.
 */
Matrix generate_sequential_mat(size_t n);


/**
 * @brief Initialize zeros matrix of size n.
 * 
 * @param n Size of the matrix
 * @return matrix Generated square matrix.
 */
Matrix generate_zeros_mat(size_t nrow, size_t ncol);


Matrix generate_random_mat(size_t nrow, size_t ncol);

/**
 * @brief Free the memory allocated for the matrix.
 * 
 * @param mat Pointer to the matrix.
 */
void destroy_mat(Matrix mat);

#endif
