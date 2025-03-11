#include <stdio.h>
#include <time.h>

#include "generate_mat/generate_mat.h"
#include "utils/helper_func.h"
#include "calc/calc.h"


typedef struct {
    size_t rows;
    size_t cols;
    int threads;
} MatrixConfig;


MatrixConfig parse_args(int argc, char const *argv[])
{
    MatrixConfig config = {0, 0, 0};

    if (argc != 4) {
        fprintf(stderr, "Usage: %s <rows> <cols> <threads>\n", argv[0]);
        return config;
    }

    config.rows = (size_t)atoi(argv[1]);
    config.cols = (size_t)atoi(argv[2]);
    config.threads = atoi(argv[3]);

    if (config.rows <= 0 || config.cols <= 0 || config.threads <= 0) {
        fprintf(stderr, "Error: Rows, columns, and threads must be positive integers.\n");
        return config;
    }

    return config;
}


int main(int argc, char const *argv[])
{
    MatrixConfig config = parse_args(argc, argv);
    const size_t n = config.rows;
    Matrix mat_A = generate_sequential_mat(n);
    Matrix mat_B = generate_sequential_mat(n);
    Matrix mat_C = generate_zeros_mat(n);

    clock_t start, end;
    double cpu_time_used;


    start = clock();
    mat_mul_strenssen(mat_A, mat_B, &mat_C);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken Stranssen: %f seconds\n", cpu_time_used);

    print_mat(mat_C, "Stranssen");
    start = clock();
    mat_mul_naive(mat_A, mat_B, &mat_C);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Time taken naive: %f seconds\n", cpu_time_used);

    print_mat(mat_C, "Naive");
    destroy_mat(mat_A);
    destroy_mat(mat_B);
    destroy_mat(mat_C);
    return 0;
}
