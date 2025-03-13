#ifndef H_STRASSEN_H
#define H_STRASSEN_H


#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "calc.h"


#define OUTER_THRESHOLD 128
#define INNER_THRESHOLD 64
#define SCALED_THRESHOLD_FACTOR 2048 * 64


/**
 * @brief Strenssen algorithm for matrix multiplication using divide and conquer.
 *
 * @param A First input matrix.
 * @param B Second input matrix.
 * @param C Resultant matrix. If the matrix is empty, default to the zero matrix.
 */
void mat_mul_strenssen(const Matrix A, const Matrix B, Matrix* C);


/**
 * @brief Implementation of the Strassen Algorithm using smart static padding.
 *
 * @param A First input matrix.
 * @param B Second input matrix.
 * @param C Resultant matrix.
 */
void mat_mul_strassen_static(const Matrix A, const Matrix B, Matrix* C);

// static inline size_t _compute_padded_size(size_t n, size_t threshold);

// static Matrix _generate_pad_matrix(const Matrix* src, size_t padded_rows, size_t padded_cols);

// static void truncate_matrix(Matrix *dst, const Matrix src, const size_t row, const size_t col);

// static Matrix mat_view(const Matrix mat, const size_t i, const size_t j, const size_t nrow, const size_t ncol);

void _mat_mul_strassen_static(const Matrix A, const Matrix B, Matrix* C);

#endif