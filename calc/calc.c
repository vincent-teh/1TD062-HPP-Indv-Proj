#include "calc.h"


void mat_mul_naive(Matrix mat_A, Matrix mat_B, Matrix* mat_C)
{
    if (mat_A.col != mat_B.row) {
        printf("Error row and columns of the two matrix does not matched!\n");
        return;
    }

    for (size_t i = 0; i < mat_A.row; i++) {
        for (size_t j = 0; j < mat_A.col; j++) {
            mat_C->matrix[i][j] = 0;
            for (size_t k = 0; k < mat_B.row; k++) {
                mat_C->matrix[i][j] += mat_A.matrix[i][k] * mat_B.matrix[k][j];
            }
        }
    }
}


void mat_add(Matrix mat_A, Matrix mat_B, Matrix* mat_C)
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
            mat_C->matrix[i][j] = mat_A.matrix[i][j] + mat_B.matrix[i][j];
    return;
}


void mat_sub(Matrix mat_A, Matrix mat_B, Matrix* mat_C)
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


void mat_mul_strenssen(Matrix A, Matrix B, Matrix* C)
{
    if (A.col != B.row) {
        printf("Error! Row & Columns of the multiplication does not agree!\n");
        return;
    }
    if ((C->row != A.row) || (C->col != A.col)) {
        printf("The size of the resultant matrix must agree with the input!\n");
        return;
    }
    if (A.col < 10) {
        mat_mul_naive(A, B, C);
        return;
    }

    size_t split_idx = A.row / 2;

    Matrix a_sub[4];
    Matrix b_sub[4];
    Matrix c_sub[4];
    Matrix M_sub[7];
    for (int i=0; i<4; i++) {
        a_sub[i] = generate_zeros_mat(split_idx);
        b_sub[i] = generate_zeros_mat(split_idx);
        c_sub[i] = generate_zeros_mat(split_idx);
    }
    for (int i=0; i<7; i++)
        M_sub[i] = generate_zeros_mat(split_idx);

    for (size_t i = 0; i < split_idx; i++)
        for (size_t j = 0; j < split_idx; j++) {
            a_sub[0].matrix[i][j] = A.matrix[i][j];
            a_sub[1].matrix[i][j] = A.matrix[i][j + split_idx];
            a_sub[2].matrix[i][j] = A.matrix[split_idx + i][j];
            a_sub[3].matrix[i][j] = A.matrix[i + split_idx][j + split_idx];

            b_sub[0].matrix[i][j] = B.matrix[i][j];
            b_sub[1].matrix[i][j] = B.matrix[i][j + split_idx];
            b_sub[2].matrix[i][j] = B.matrix[split_idx + i][j];
            b_sub[3].matrix[i][j] = B.matrix[i + split_idx][j + split_idx];
        }

    Matrix temp[2];
    temp[0] = generate_zeros_mat(split_idx);
    temp[1] = generate_zeros_mat(split_idx);

    mat_add(a_sub[0], a_sub[3], &temp[0]);
    mat_add(b_sub[0], b_sub[3], &temp[1]);
    mat_mul_strenssen(temp[0], temp[1], &M_sub[0]);

    mat_add(a_sub[2], a_sub[3], &temp[0]);
    mat_mul_strenssen(temp[0], b_sub[0], &M_sub[1]);

    mat_sub(b_sub[1], b_sub[3], &temp[1]);
    mat_mul_strenssen(a_sub[0], temp[1], &M_sub[2]);

    mat_sub(b_sub[2], b_sub[0], &temp[1]);
    mat_mul_strenssen(a_sub[3], temp[1], &M_sub[3]);

    mat_add(a_sub[0], a_sub[1], &temp[0]);
    mat_mul_strenssen(temp[0], b_sub[3], &M_sub[4]);

    mat_sub(a_sub[2], a_sub[0], &temp[0]);
    mat_add(b_sub[0], b_sub[1], &temp[1]);
    mat_mul_strenssen(temp[0], temp[1], &M_sub[5]);

    mat_sub(a_sub[1], a_sub[3], &temp[0]);
    mat_add(b_sub[2], b_sub[3], &temp[1]);
    mat_mul_strenssen(temp[0], temp[1], &M_sub[6]);

    // Summation
    mat_add(M_sub[0], M_sub[3], &temp[0]);
    mat_sub(temp[0], M_sub[4], &temp[1]);
    mat_add(temp[1], M_sub[6], &c_sub[0]);

    mat_add(M_sub[2], M_sub[4], &c_sub[1]);

    mat_add(M_sub[1], M_sub[3], &c_sub[2]);

    mat_sub(M_sub[0], M_sub[1], &temp[0]);
    mat_add(temp[0], M_sub[2], &temp[1]);
    mat_add(temp[1], M_sub[5], &c_sub[3]);

    for (size_t i = 0; i < split_idx; i++)
        for (size_t j = 0; j < split_idx; j++) {
            C->matrix[i][j]                     = c_sub[0].matrix[i][j];
            C->matrix[i][j+split_idx]           = c_sub[1].matrix[i][j];;
            C->matrix[i+split_idx][j]           = c_sub[2].matrix[i][j];;
            C->matrix[i+split_idx][j+split_idx] = c_sub[3].matrix[i][j];;
        }

    destroy_mat(temp[0]);
    destroy_mat(temp[1]);
    for (int i=0; i<4; i++) {
        destroy_mat(a_sub[i]);
        destroy_mat(b_sub[i]);
        destroy_mat(c_sub[i]);
    }
    for (int i=0; i<7; i++)
        destroy_mat(M_sub[i]);

    return;
}
