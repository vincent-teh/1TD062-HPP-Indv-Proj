#include "strassen.h"


static inline bool should_use_naive(size_t m, size_t n, size_t p)
{
    // Compute the effective size
    size_t effective_size = m * n * p;

    // If any dimension is very small, use naive multiplication
    if (m <= OUTER_THRESHOLD || p <= OUTER_THRESHOLD || n <= INNER_THRESHOLD) {
        return true;
    }

    // If the effective size is below a scaled threshold, use naive multiplication
    if (effective_size <= SCALED_THRESHOLD_FACTOR) {
        return true;
    }

    // Otherwise, use Strassen's algorithm
    return false;
}


void mat_mul_strenssen(const Matrix A, const Matrix B, Matrix* C)
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



static inline size_t _compute_padded_size(size_t n, size_t threshold)
{
    if (n <= threshold) return n;

    size_t k = ceil(log2((double)n / threshold)); // Number of splits needed
    return threshold * (1 << k); // Equivalent to threshold * 2^k
}

static Matrix _generate_pad_matrix(const Matrix* src,
                                  size_t padded_rows, size_t padded_cols) {
    // Safety checks
    if (padded_rows < src->row || padded_cols < src->col) {
        fprintf(stderr, "Padded dimensions must be >= original\n");
        return (Matrix){NULL, 0, 0};
    }

    Matrix padded = generate_zeros_mat(padded_rows, padded_cols);
    if (!padded.matrix) return padded; // Handle allocation failure

    // Copy row by row using 2D array semantics
    for (size_t i = 0; i < src->row; i++) {
        memcpy(
            padded.matrix[i],          // Destination: start of row i in padded
            src->matrix[i],            // Source: start of row i in original
            src->col * sizeof(double) // Copy only original columns
        );
        // Remaining columns in padded row are already zero-initialized
    }

    return padded;
}


static Matrix mat_view(const Matrix mat, const size_t i, const size_t j, const size_t nrow, const size_t ncol)
{
    // Bounds checking (optional but recommended)
    if (i >= mat.row || j >= mat.col || nrow == 0 || ncol == 0) {
        fprintf(stderr, "Invalid view parameters\n");
        return (Matrix){NULL, 0, 0};
    }

    // Allocate array of row pointers for the view
    double** view_rows = malloc(nrow * sizeof(double*));
    if (!view_rows) {
        fprintf(stderr, "Memory allocation failed\n");
        return (Matrix){NULL, 0, 0};
    }

    // Point each view row to the corresponding original row + column offset
    for (size_t k = 0; k < nrow; k++) {
        size_t orig_row = i + k;
        if (orig_row >= mat.row) { // Handle out-of-bounds rows
            view_rows[k] = NULL; // Or pad with zeros
        } else {
            view_rows[k] = &mat.matrix[orig_row][j]; // Column offset
        }
    }

    return (Matrix){.matrix = view_rows, .row = nrow, .col = ncol};
}


static void truncate_matrix(Matrix *dst, const Matrix src, const size_t row, const size_t col)
{
    for (size_t i = 0; i < row; i++) {
        for (size_t j=0; j < col; j++) {
            dst->matrix[i][j] = src.matrix[i][j];
        }
    }
}


void mat_mul_strassen_static(const Matrix A, const Matrix B, Matrix* C)
{
    if (A.col != B.row) {
        printf("Error! Row & Columns of the multiplication does not agree!\n");
        return;
    }

    if ((C->row != A.row) || (C->col != B.col)) {
        printf("The size of the resultant matrix must agree with the input!\n");
        return;
    }

    const size_t A_padded_rows = _compute_padded_size(A.row, OUTER_THRESHOLD);
    const size_t A_padded_cols = _compute_padded_size(A.col, INNER_THRESHOLD);
    const size_t B_padded_cols = _compute_padded_size(B.col, OUTER_THRESHOLD);

    const Matrix A_padded = ((A_padded_rows != A.row) || (A_padded_cols != A.col))
    ? _generate_pad_matrix(&A, A_padded_rows, A_padded_cols)
    : A;

    const Matrix B_padded = ((A_padded_cols != B.row) || (B_padded_cols != B.col))
    ? _generate_pad_matrix(&B, A_padded_cols, B_padded_cols)
    : B;

    Matrix C_padded = generate_zeros_mat(A_padded_rows, B_padded_cols);

    _mat_mul_strassen_static(A_padded, B_padded, &C_padded);

    truncate_matrix(C, C_padded, A.row, B.col);


    destroy_mat(C_padded);
    if ((A_padded_rows != A.row) || (A_padded_cols != A.col)) {
        destroy_mat(A_padded);
    }
    if ((A_padded_cols != B.row) || (B_padded_cols != B.col)) {
        destroy_mat(B_padded);
    }
}


void _mat_mul_strassen_static(const Matrix A, const Matrix B, Matrix* C)
{
#ifdef DEBUG
    if (A.col != B.row) {
        printf("Error! Row & Columns of the multiplication does not agree! ARow: %ld BCol: %ld\n", A.col, B.row);
        return;
    }
    if ((C->row != A.row) || (C->col != B.col)) {
        printf("The size of the resultant matrix must agree with the input!\n");
        return;
    }
#endif

    if (should_use_naive(A.row, A.col, B.col)) {
        mat_mul_naive(A, B, C);
        return;
    }

    size_t split_row_A = A.row / 2;
    size_t split_col_A_row_B  = A.col / 2;
    size_t split_col_B = B.col / 2;

    Matrix a_sub[4];
    Matrix b_sub[4];
    Matrix c_sub[4];
    Matrix M_sub[7];
    for (int i=0; i<2; i++)
        for (int j=0; j<2; j++) {
            a_sub[i*2+j] = mat_view(A, i*split_row_A, j*split_col_A_row_B, split_row_A, split_col_A_row_B);
            b_sub[i*2+j] = mat_view(B, i*split_col_A_row_B, j*split_col_B, split_col_A_row_B, split_col_B);
            c_sub[i*2+j] = mat_view(*C, i*split_row_A, j*split_col_B, split_row_A, split_col_B);
        }

    for (int i=0; i<7; i++)
        M_sub[i] = generate_zeros_mat(split_row_A, split_col_B);

    Matrix temp_a = generate_zeros_mat(split_row_A, split_col_A_row_B);
    Matrix temp_b = generate_zeros_mat(split_col_A_row_B, split_col_B);

    mat_add(a_sub[0], a_sub[3], &temp_a);
    mat_add(b_sub[0], b_sub[3], &temp_b);
    _mat_mul_strassen_static(temp_a, temp_b, &M_sub[0]);

    mat_add(a_sub[2], a_sub[3], &temp_a);
    _mat_mul_strassen_static(temp_a, b_sub[0], &M_sub[1]);

    mat_sub(b_sub[1], b_sub[3], &temp_b);
    _mat_mul_strassen_static(a_sub[0], temp_b, &M_sub[2]);

    mat_sub(b_sub[2], b_sub[0], &temp_b);
    _mat_mul_strassen_static(a_sub[3], temp_b, &M_sub[3]);

    mat_add(a_sub[0], a_sub[1], &temp_a);
    _mat_mul_strassen_static(temp_a, b_sub[3], &M_sub[4]);

    mat_sub(a_sub[2], a_sub[0], &temp_a);
    mat_add(b_sub[0], b_sub[1], &temp_b);
    _mat_mul_strassen_static(temp_a, temp_b, &M_sub[5]);

    mat_sub(a_sub[1], a_sub[3], &temp_a);
    mat_add(b_sub[2], b_sub[3], &temp_b);
    _mat_mul_strassen_static(temp_a, temp_b, &M_sub[6]);


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

    for (int i=0; i<4; i++) {
        free(a_sub[i].matrix);
        free(b_sub[i].matrix);
        free(c_sub[i].matrix);
    }

    destroy_mat(temp_a);
    destroy_mat(temp_b);
    destroy_mat(temp_c[0]);
    destroy_mat(temp_c[1]);

    for (int i=0; i<7; i++) {
        destroy_mat(M_sub[i]);
    }
    return;
}