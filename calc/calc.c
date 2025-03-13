#include "calc.h"


// void mat_mul_naive(Matrix mat_A, Matrix mat_B, Matrix* mat_C)
// {
//     if (mat_A.col != mat_B.row) {
//         printf("Error row and columns of the two matrix does not matched!\n");
//         return;
//     }

//     for (size_t i = 0; i < mat_A.row; i++) {
//         for (size_t j = 0; j < mat_B.col; j++) {
//             mat_C->matrix[i][j] = 0;
//             for (size_t k = 0; k < mat_B.row; k++) {
//                 mat_C->matrix[i][j] += mat_A.matrix[i][k] * mat_B.matrix[k][j];
//             }
//         }
//     }
// }


void mat_mul_naive(Matrix mat_A, Matrix mat_B, Matrix* mat_C)
{
    if (mat_A.col != mat_B.row) {
        printf("Error: Dimension mismatch for multiplication!\n");
        return;
    }

    // Initialize result matrix to 0
    for (size_t i = 0; i < mat_C->row; i++) {
        memset(mat_C->matrix[i], 0, mat_C->col*sizeof(mat_C->matrix[i][0]));
    }

    // Optimal block size (tune based on CPU cache, typically 64-128)
    const size_t BLOCK_SIZE = 64;

    // Blocked matrix multiplication
    for (size_t i_block = 0; i_block < mat_A.row; i_block += BLOCK_SIZE) {
        size_t i_end = i_block + BLOCK_SIZE;
        if (i_end > mat_A.row) i_end = mat_A.row;

        for (size_t j_block = 0; j_block < mat_B.col; j_block += BLOCK_SIZE) {
            size_t j_end = j_block + BLOCK_SIZE;
            if (j_end > mat_B.col) j_end = mat_B.col;

            for (size_t k_block = 0; k_block < mat_A.col; k_block += BLOCK_SIZE) {
                size_t k_end = k_block + BLOCK_SIZE;
                if (k_end > mat_A.col) k_end = mat_A.col;

                // Multiply block (i_block x k_block) of A with (k_block x j_block) of B
                for (size_t i = i_block; i < i_end; i++) {
                    for (size_t k = k_block; k < k_end; k++) {
                        const double a_val = mat_A.matrix[i][k];
                        for (size_t j = j_block; j < j_end; j++) {
                            mat_C->matrix[i][j] += a_val * mat_B.matrix[k][j];
                        }
                    }
                }
            }
        }
    }
}


void mat_add(const Matrix mat_A, const Matrix mat_B, Matrix* mat_C)
{
#ifdef DEBUG
    if ((mat_A.row != mat_B.row) || (mat_A.col != mat_B.col)) {
        printf("The size of the matrix must matched to perform addition!\n");
        return;
    }

    if ((mat_C->row != mat_A.row) || (mat_C->col != mat_A.col)) {
        printf("The size of the resultant matrix must agree with the input!\n");
        return;
    }
#endif

    for (size_t i=0; i<mat_A.row; i++)
        for(size_t j=0; j<mat_A.col; j++)
            mat_C->matrix[i][j] = mat_A.matrix[i][j] + mat_B.matrix[i][j];
    return;
}


void mat_sub(const Matrix mat_A, const Matrix mat_B, Matrix* mat_C)
{
    if ((mat_A.row != mat_B.row) || (mat_A.col != mat_B.col)) {
        printf("The size of the matrix must matched to perform addition!\n");
        return;
    }

    if ((mat_C->row != mat_A.row) || (mat_C->col != mat_A.col)) {
        printf("The size of the resultant matrix must agree with the input!\n");
        return;
    }

    for (size_t i=0; i<mat_A.row; i++)
        for(size_t j=0; j<mat_A.col; j++)
            mat_C->matrix[i][j] = mat_A.matrix[i][j] - mat_B.matrix[i][j];
    return;
}
