#ifndef H_CALC_H
#define H_CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../generate_mat/generate_mat.h"
#include "../utils/helper_func.h"

#define NORMAL_MUL_THRESHOLD 128


/**
 * @brief Multiplication of 2 square matrices of the same size.
 * 
 * @param mat_A A matrix, n*n.
 * @param n Size of the both matrix.
 * @param B B matrix, n*n.
 * @return matrix 
 */
void mat_mul_naive(Matrix mat_A, Matrix mat_B, Matrix* mat_C);


/**
 * @brief Performs addition of the two matrices. If matrix C is a null matrix,
 *        then it will allocate memories for it.
 *
 * @param mat_A First matrix to be summed.
 * @param mat_B Second matrix to be summed.
 * @param mat_C Resulten matrix of the summed.
 */
void mat_add(Matrix mat_A, Matrix mat_B, Matrix* mat_C);


void mat_sub(Matrix mat_A, Matrix mat_B, Matrix* mat_C);

/**
 * @brief Strenssen algorithm for matrix multiplication using divide and conquer.
 *
 * @param A First input matrix.
 * @param B Second input matrix.
 * @param C Resultant matrix. If the matrix is empty, default to the zero matrix.
 */
void mat_mul_strenssen(Matrix A, Matrix B, Matrix* C);

#endif