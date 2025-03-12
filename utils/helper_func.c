#include "helper_func.h"


void print_mat(const Matrix mat, const char* name)
{
    printf("=======Matrix %s========\n", name);
    size_t rows = (mat.row < 10) ? mat.row : 10;
    size_t cols = (mat.col < 10) ? mat.col : 10;
    if (mat.row > 10 || mat.col > 10) printf("Printing truncated matrix\n");
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            printf("%3.3f ", mat.matrix[i][j]);
        }
        printf("\n");
    }
}
