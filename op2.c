
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>

void op2(float* a_vec, int m, int n , int p , float *b_vec, float *c_vec) 
{
    float (*a)[n] = (float(*)[n]) a_vec;
    float (*b)[p] = (float(*)[p]) b_vec;
    float (*c)[p] = (float(*)[p]) c_vec;
    // Your code starts here
double start, end;
float *temp = calloc(1, sizeof(float));
  start = omp_get_wtime(); // save start time in to variable 'start'
for (int i = 0; i <m;i++)
{
    for (int j = 0; j <p;j++)
    {
    
        for (int k = 0; k <n;k++)
        {
        *temp += a[i][k] * b[k][j];
        }        
        c[i][j]=*temp;
        *temp=0;
    }
}
end = omp_get_wtime();
free(temp);
printf("Time: %f milliseconds on threads \n",1000.0*(end-start));
}


