#ifndef H_CALC_H
#define H_CALC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../generate_mat/generate_mat.h"
#include "../utils/helper_func.h"


#define DEBUG

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
void mat_add(const Matrix mat_A, const Matrix mat_B, Matrix* mat_C);

/**
 * @brief Matrix subtraction.
 *
 * @param mat_A First matrix to be subtracted.
 * @param mat_B Second matrix to be subtracted.
 * @param mat_C Resultan matrix.
 */
void mat_sub(const Matrix mat_A, const Matrix mat_B, Matrix* mat_C);



#endif