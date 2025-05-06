#ifndef UWB_Matrix
#define UWB_Matrix

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include"UWB_Math.h"
#include"UWB_Matrix.c"

#define matrix_size 3

void find_3b3_Inverse_Matrix(float *divided_array, float input[matrix_size][matrix_size]);
void matrix_3b3_3_Multiplication(float *drifferent_array, float partial_array[matrix_size][matrix_size], float function_array[matrix_size]);

#ifdef __cplusplus
}
#endif

#endif
