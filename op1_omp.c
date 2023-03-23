#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <math.h>

void op1(float *input_vec, const int m, const int n, float *filter_vec, int k, float *output_vec, int b)
{
    float(*input)[m][n] = (float(*)[m][n]) input_vec;
    float(*filter)[k] = (float(*)[k]) filter_vec;
    float(*output)[m][n] = (float(*)[m][n]) output_vec;
    // Your code starts here

int numThreads=-1;
double start;
int offset1, offset2;

//get start time
start=omp_get_wtime();

// set offset values based on k size (even/odd)
// if size of the filter is odd
if (k  % 2 != 0)
{
offset1 = (k  / 2);
offset2 = (k  / 2);
}
// if size of the filter is even
else
{
offset1 = (k  / 2)-1;
offset2 = (k  / 2);
}

#pragma omp parallel default(none) shared(input,output,filter,offset1,offset2,k,b,numThreads,start)
{
//create temp variable
float temp;
numThreads = omp_get_num_threads();
// loop for b - number of batches
#pragma omp for schedule(static) collapse(2) nowait
    for (int z = 0; z < b; ++z)
    {
        //loop to iterate through m
        for (int y = 0; y < m; ++y)
        {
            //loop to iterate through n
            for (int x = 0; x < n; ++x)
            {
                //if statement to ensure that filter is not applied to values in first and last column/row
                if (x>=(offset1) && y>=(offset1) && x<(n-offset2) && y<(m-offset2))
                {
                    //null temp variable
                    temp = 0;
                    
                        //iteration through the filter
                        for (int i = 0; i < (k*k); i++)
                        {   
                            
                            // s is equal to line on which we want to work                
                            int s = (i/k);
                            //calculate values multipled by filter and add them to the temp variable
                            temp+= (input[z][y-offset1+s][x-offset1+i-(k*s)] * filter[s][i-(k*s)]);
                        }

                    //move variables from temp and flush temp variable
                    output[z][y][x]=temp;
                }
                else
                {
                //else just copy values from input;
                output[z][y][x] = input[z][y][x];
                }
            }              
        }
    }    
#pragma omp barrier
#pragma omp master
{
printf("OMP time: %f milliseconds: (%d threads)\n",1000.*(omp_get_wtime()-start),numThreads); 
}
}




return;
}






