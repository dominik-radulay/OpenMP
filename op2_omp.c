
#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

void op2(float* a_vec, int m, int n , int p , float *b_vec, float *c_vec) 
{
    float (*a)[n] = (float(*)[n]) a_vec;
    float (*b)[p] = (float(*)[p]) b_vec;
    float (*c)[p] = (float(*)[p]) c_vec;
    // Your code starts here
int numThreads=-1;
double start;
start=omp_get_wtime();

#pragma omp parallel default(none) shared(m,n,start,a,b,c,p,numThreads)
{
float temp;
numThreads = omp_get_num_threads();
  #pragma omp for schedule(static) collapse(2) nowait
  for (int i = 0; i <m;i++)
  {
    for (int j = 0; j <p;j++)
    {
      temp =0;
      for (int k = 0; k <n;k++)
      {
        temp += a[i][k] * b[k][j];
      }        
      c[i][j]=temp;
    }
  }
#pragma omp barrier
#pragma omp master
{
printf("OMP time: %f milliseconds: (%d threads)\n",1000.*(omp_get_wtime()-start),numThreads); 
}
}

}


