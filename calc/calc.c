#include "calc.h"


static inline bool should_use_naive(size_t m, size_t n, size_t p)
{
    // Compute the effective size
    size_t effective_size = m * n * p;

    // If any dimension is very small, use naive multiplication
    if (m <= OUTER_THRESHOLD || p <= OUTER_THRESHOLD || n <= INNER_THRESHOLD) {
        return true;
    }

    // If the effective size is below a scaled threshold, use naive multiplication
    if (effective_size <= SCALED_THRESHOLD_FACTOR * INNER_THRESHOLD) {
        return true;
    }

    // Otherwise, use Strassen's algorithm
    return false;
}

void mat_mul_naive(Matrix mat_A, Matrix mat_B, Matrix* mat_C)
{
    if (mat_A.col != mat_B.row) {
        printf("Error row and columns of the two matrix does not matched!\n");
        return;
    }

    for (size_t i = 0; i < mat_A.row; i++) {
        for (size_t j = 0; j < mat_B.col; j++) {
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
    if ((C->row != A.row) || (C->col != B.col)) {
        printf("The size of the resultant matrix must agree with the input!\n");
        return;
    }
    if (should_use_naive(A.row, A.col, B.col)) {
        mat_mul_naive(A, B, C);
        return;
    }

    size_t split_row_A = (A.row + 1) / 2;
    size_t split_col_A_row_B  = (A.col + 1) / 2; // Ceiling division.
    size_t split_col_B = (B.col + 1) / 2;

    Matrix a_sub[4];
    Matrix b_sub[4];
    Matrix c_sub[4];
    Matrix M_sub[7];
    for (int i=0; i<4; i++) {
        a_sub[i] = generate_zeros_mat(split_row_A, split_col_A_row_B);
        b_sub[i] = generate_zeros_mat(split_col_A_row_B, split_col_B);
        c_sub[i] = generate_zeros_mat(split_row_A, split_col_B);
    }
    for (int i=0; i<7; i++)
        M_sub[i] = generate_zeros_mat(split_row_A, split_col_B);

    for (size_t i = 0; i < split_row_A; i++)
        for (size_t j = 0; j < split_col_A_row_B; j++) {
            a_sub[0].matrix[i][j] = A.matrix[i][j];
            a_sub[1].matrix[i][j] = (j + split_col_A_row_B < A.col) ? A.matrix[i][j + split_col_A_row_B] : 0;
            a_sub[2].matrix[i][j] = (i + split_row_A < A.row) ? A.matrix[i + split_row_A][j] : 0;
            a_sub[3].matrix[i][j] = (i + split_row_A < A.row && j + split_col_A_row_B < A.col)
                                        ? A.matrix[i + split_row_A][j + split_col_A_row_B] : 0;
        }

    for (size_t i = 0; i < split_col_A_row_B; i++) {
        for (size_t j = 0; j < split_col_B; j++) {
            b_sub[0].matrix[i][j] = B.matrix[i][j];
            b_sub[1].matrix[i][j] = (j + split_col_B < B.col) ? B.matrix[i][j + split_col_B] : 0;
            b_sub[2].matrix[i][j] = (i + split_col_A_row_B < B.row) ? B.matrix[i + split_col_A_row_B][j] : 0;
            b_sub[3].matrix[i][j] = (i + split_col_A_row_B < B.row && j + split_col_B < B.col)
                                        ? B.matrix[i + split_col_A_row_B][j + split_col_B] : 0;
        }
    }

    Matrix temp_a = generate_zeros_mat(split_row_A, split_col_A_row_B);
    Matrix temp_b = generate_zeros_mat(split_col_A_row_B, split_col_B);

    mat_add(a_sub[0], a_sub[3], &temp_a);
    mat_add(b_sub[0], b_sub[3], &temp_b);
    mat_mul_strenssen(temp_a, temp_b, &M_sub[0]);

    mat_add(a_sub[2], a_sub[3], &temp_a);
    mat_mul_strenssen(temp_a, b_sub[0], &M_sub[1]);

    mat_sub(b_sub[1], b_sub[3], &temp_b);
    mat_mul_strenssen(a_sub[0], temp_b, &M_sub[2]);

    mat_sub(b_sub[2], b_sub[0], &temp_b);
    mat_mul_strenssen(a_sub[3], temp_b, &M_sub[3]);

    mat_add(a_sub[0], a_sub[1], &temp_a);
    mat_mul_strenssen(temp_a, b_sub[3], &M_sub[4]);

    mat_sub(a_sub[2], a_sub[0], &temp_a);
    mat_add(b_sub[0], b_sub[1], &temp_b);
    mat_mul_strenssen(temp_a, temp_b, &M_sub[5]);

    mat_sub(a_sub[1], a_sub[3], &temp_a);
    mat_add(b_sub[2], b_sub[3], &temp_b);
    mat_mul_strenssen(temp_a, temp_b, &M_sub[6]);

    Matrix temp_c[2];
    temp_c[0] = generate_zeros_mat(split_row_A, split_col_B);
    temp_c[1] = generate_zeros_mat(split_row_A, split_col_B);

    // Summation
    mat_add(M_sub[0], M_sub[3], &temp_c[0]);
    mat_sub(temp_c[0], M_sub[4], &temp_c[1]);
    mat_add(temp_c[1], M_sub[6], &c_sub[0]);

    mat_add(M_sub[2], M_sub[4], &c_sub[1]);

    mat_add(M_sub[1], M_sub[3], &c_sub[2]);

    mat_sub(M_sub[0], M_sub[1], &temp_c[0]);
    mat_add(temp_c[0], M_sub[2], &temp_c[1]);
    mat_add(temp_c[1], M_sub[5], &c_sub[3]);

    for (size_t i = 0; i < split_row_A; i++)
        for (size_t j = 0; j < split_col_B; j++) {
            C->matrix[i][j] = c_sub[0].matrix[i][j];
            if (j+split_col_B < C->col) C->matrix[i][j+split_col_B] = c_sub[1].matrix[i][j];;
            if (i+split_row_A < C->row) C->matrix[i+split_row_A][j] = c_sub[2].matrix[i][j];;
            if (i+split_row_A < C->row && j+split_col_B < C->col)
                C->matrix[i+split_row_A][j+split_col_B] = c_sub[3].matrix[i][j];;
        }

    destroy_mat(temp_a);
    destroy_mat(temp_b);
    destroy_mat(temp_c[0]);
    destroy_mat(temp_c[1]);
    for (int i=0; i<4; i++) {
        destroy_mat(a_sub[i]);
        destroy_mat(b_sub[i]);
        destroy_mat(c_sub[i]);
    }
    for (int i=0; i<7; i++)
        destroy_mat(M_sub[i]);

    return;
}
