#include <stdio.h>
#include <time.h>

#include "generate_mat/generate_mat.h"
#include "utils/helper_func.h"
#include "calc/calc.h"
#include "calc/strassen.h"


#define PRINTING 0

typedef struct {
    size_t rows_A;
    size_t cols_A_row_B;
    size_t cols_B;
    int threads;
} MatrixConfig;


MatrixConfig parse_args(int argc, char const *argv[])
{
    MatrixConfig config = {0, 0, 0, 0};

    if (argc != 5) {
        fprintf(stderr, "Usage: %s <rows A> <cols A rows B> <cols B> <threads>\n", argv[0]);
        return config;
    }

    config.rows_A = (size_t)atoi(argv[1]);
    config.cols_A_row_B = (size_t)atoi(argv[2]);
    config.cols_B = (size_t)atoi(argv[3]);
    config.threads = atoi(argv[4]);

    if (config.rows_A <= 0 || config.cols_A_row_B <= 0 || config.cols_B <=0 || config.threads <= 0) {
        fprintf(stderr, "Error: Rows, columns, and threads must be positive integers.\n");
        return config;
    }

    return config;
}


int main(int argc, char const *argv[])
{
    MatrixConfig config = parse_args(argc, argv);
    // Matrix mat_A = generate_sequential_mat(config.rows_A, config.cols_A_row_B);
    // Matrix mat_B = generate_sequential_mat(config.cols_A_row_B, config.cols_B);
    Matrix mat_A = generate_random_mat(config.rows_A, config.cols_A_row_B);
    Matrix mat_B = generate_random_mat(config.cols_A_row_B, config.cols_B);
    Matrix mat_C = generate_zeros_mat(config.rows_A, config.cols_B);

    clock_t start, end;
    double cpu_time_used;


    if (PRINTING) {
        print_mat(mat_A, "A");
        print_mat(mat_B, "B");
    }

#if 1
    start = clock();
    mat_mul_naive(mat_A, mat_B, &mat_C);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken naive: %f seconds\n", cpu_time_used);
    if (PRINTING) print_mat(mat_C, "Naive");
#endif

#if 1
    start = clock();
    mat_mul_strenssen(mat_A, mat_B, &mat_C);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken Stranssen: %f seconds\n", cpu_time_used);
    if (PRINTING) print_mat(mat_C, "Stranssen");
#endif


#if 1
    start = clock();
    mat_mul_strassen_static(mat_A, mat_B, &mat_C);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken Stranssen: %f seconds\n", cpu_time_used);
    if (PRINTING) print_mat(mat_C, "Stranssen");
#endif

    destroy_mat(mat_A);
    destroy_mat(mat_B);
    destroy_mat(mat_C);

    return 0;
}
