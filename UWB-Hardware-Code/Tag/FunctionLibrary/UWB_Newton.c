#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"UWB_Matrix.h"

#define matrix_size 3

void newton_3b4_3(  float *guess, 
                    float anchors_location[4][matrix_size], 
                    float distance[matrix_size]
                ){
    // function matrix
    float xmx = *(guess+0)-anchors_location[3][0];
    float ymy = *(guess+1)-anchors_location[3][1];
    float zmz = *(guess+2)-anchors_location[3][2];
    float r = distance[3];
    float function_array_m = (xmx*xmx)+(ymy*ymy)+(zmz*zmz)-(r*r);

    float function_array[matrix_size] = {0.0};
    for(int i = 0; i < matrix_size; i++){
        float xmx = *(guess+0)-anchors_location[i][0];
        float ymy = *(guess+1)-anchors_location[i][1];
        float zmz = *(guess+2)-anchors_location[i][2];
        float r = distance[i];
        function_array[i] = (
            ((xmx*xmx)+(ymy*ymy)+(zmz*zmz)-(r*r))-
            function_array_m
        );
    }

    // partial matrix
    float partial_array[matrix_size][matrix_size] = {0.0};
    for(int i = 0; i < matrix_size; i++)
        for(int j = 0; j < matrix_size; j++)
            partial_array[i][j] =
                2*(anchors_location[3][j]-anchors_location[i][j]);

    // inverse partial matrix
    float Inverse_Array_P[matrix_size*matrix_size] = {0.0};
    find_3b3_Inverse_Matrix(Inverse_Array_P, partial_array);
    float Inverse_Array[matrix_size][matrix_size] = {0.0};
    memcpy(Inverse_Array, Inverse_Array_P, sizeof(float)*matrix_size*matrix_size);

    // drifferent
    float current_drifferent[matrix_size] = {0.0};
    matrix_3b3_3_Multiplication(current_drifferent, Inverse_Array, function_array);

    // global Newton
    float current_round[matrix_size] = {0.0};
    float function_array_out[matrix_size] = {0.0};
    float a = 1;
    for(int i = 0; i < 40; i++) {
        // offset
        for(int j = 0; j < matrix_size; j++)
            current_round[j] = *(guess+j)-(a*current_drifferent[j]);
        for(int j = 0; j < matrix_size; j++ ) {
            float xmx = current_round[0]-anchors_location[j][0];
            float ymy = current_round[1]-anchors_location[j][1];
            float zmz = current_round[2]-anchors_location[j][2];
            float r = distance[j];
            function_array_out[j] = (xmx*xmx)+(ymy*ymy)+(zmz*zmz)-(r*r);
        }
        int fout_is_less_than_fin = (
            (abs(function_array_out[0]) < abs(function_array[0])) +
            (abs(function_array_out[1]) < abs(function_array[1])) +
            (abs(function_array_out[2]) < abs(function_array[2]))
        );
        if(fout_is_less_than_fin >= 1){
            memcpy(guess, current_round, sizeof(float)*matrix_size);
            return;
        }
        a = a/2;
    }

    return;
}