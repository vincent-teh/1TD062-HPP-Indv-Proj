#include "helper_func.h"


void print_mat(const Matrix mat, const char* name) 
{
    printf("=======Matrix %s========\n", name);
    for (size_t i = 0; i < mat.row; i++) {
        for (size_t j = 0; j < mat.col; j++) {
            printf("%3.3f ", mat.matrix[i][j]);
        }
        printf("\n");
    }
}
