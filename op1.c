#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

void op1(float *input_vec,  int m,  int n, float *filter_vec, int k, float *output_vec, int b)
{
    float(*input)[m][n] = (float(*)[m][n]) input_vec;
    float(*filter)[k] = (float(*)[k]) filter_vec;
    float(*output)[m][n] = (float(*)[m][n]) output_vec;
    // Your code starts here

double start, end;

int offset1 = 0;
int offset2 = 0;
float temp;
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

int y,x,i,z;
  start = omp_get_wtime(); // save start time in to variable 'start'

// loop for b - number of batches
   for (z = 0; z < b; ++z)
    {
        //loop to iterate through m
        for (y = 0; y < m; ++y)
        {;
            //loop to iterate through n
            for (x = 0; x < n; ++x)
            { 
                //if statement to ensure that filter is not applied to values in first and last column/row
                if (x>=(offset1) && y>=(offset1) && x<(n-offset2) && y<(m-offset2))
                {
                    //create temp variable
                    float *temp = calloc(1, sizeof(float));

                    //iteration through the filter
                    for (i = 0; i < (k*k); i++)
                    {   
                        // s is equal to line on which we want to work                
                        int s = (i/k);
                        //calculate values multipled by filter and add them to the temp variable
                        *temp+= (input[z][y-offset1+s][x-offset1+i-(k*s)] * filter[s][i-(k*s)]);
                    }
                    //move variables from temp and flush temp variable
                    output[z][y][x]=*temp;
                    free(temp);
                }
                else
                {
                //else just copy values from input;
                output[z][y][x] = input[z][y][x];
                }
            }
        }
    }
    //get time and print it 
  end = omp_get_wtime();
  printf("Time: %f milliseconds on threads \n",1000.0*(end-start));
return;
}






