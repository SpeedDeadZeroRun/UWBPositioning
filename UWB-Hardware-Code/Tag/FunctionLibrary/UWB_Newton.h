#ifndef UWB_Newton
#define UWB_Newton

#ifdef __cplusplus
extern "C" {
#endif

#include<stdlib.h>
#include"UWB_Matrix.h"
#include"UWB_Newton.c"

#define matrix_size 3

void newton_3b4_3(float *guess, float anchors_location[4][matrix_size], float distance[matrix_size]);

#ifdef __cplusplus
}
#endif

#endif
