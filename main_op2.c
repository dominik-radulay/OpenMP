#include<stdio.h>
#include<stdlib.h>
#define __USE_C99_MATH
#include<string.h>
#include <stdbool.h>
#include<math.h>

#include <stdbool.h>

bool test_case1();

void op2(float*, int, int, int, float*, float*);


int *read_dims(char *filename) {
    FILE *file = fopen(filename,"r");
    
    if(file == NULL) {
        printf("Unable to open file: %s", filename);
        return NULL;
    }

    char firstline[500];
    fgets(firstline, 500, file);
    
    int line_length = strlen(firstline);

    int num_dims = 0;
    for(int i=0; i<line_length; i++) {
        if(firstline[i] == ' ') {
            num_dims++;
        }
    }
    
    int *dims = malloc((num_dims+1)*sizeof(int));
    dims[0] = num_dims;
    const char s[2] = " ";
    char *token;
    token = strtok(firstline, s);
    int i = 0;
    while( token != NULL ) {
        dims[i+1] = atoi(token);
        i++;
        token = strtok(NULL, s);
    }
    fclose(file);
    return dims;
}

long int product(int *array, int n) {
    long int product = 1;
    for(int i=0; i<n; i++) {
        product *= array[i];
    }
    return product;
}

float * read_array(char *filename, int *dims, int num_dims) {
    FILE *file = fopen(filename,"r");

    if(file == NULL) {
        printf("Unable to open file: %s", filename);
        return NULL;
    }

    char firstline[500];
    fgets(firstline, 500, file);

    //Ignore first line and move on since first line contains 
    //header information and we already have that. 

    long int total_elements = product(dims, num_dims);

    float *one_d = malloc(sizeof(float) * total_elements);

    for(int i=0; i<total_elements; i++) {
        fscanf(file, "%f", &one_d[i]);
    }
    fclose(file);
    return one_d;
}





int main(int argc, char *argv[]) {
     if(argc != 4) {
        printf("Usage: %s <filename_input> <filename_kernel> <filename_expected_output>\n", argv[0]);
        return -1;
    }
    
    // Setting to 0 will write the expected output to the file specified as the third parameter. 
    // Setting to 1 will read the expected output from the file and compare with the given program. 
    int compareOutput = 1;

    bool match = true;

    char A_filename[500];
    char B_filename[500];
    char C_filename[500];

    strcpy(A_filename, argv[1]);
    strcpy(B_filename, argv[2]);
    strcpy(C_filename, argv[3]);


    int *A_dims_original = read_dims(A_filename);
    
    if(A_dims_original == NULL) {
        return -1;
    }

   

    int input_num_dims = A_dims_original[0];
    int *input_dims = A_dims_original+1;
    float *input_data = read_array(A_filename, input_dims, input_num_dims);
    if(input_data == NULL) {
        return -1;
    }
    
    
    int *kernel_dims_original = read_dims(B_filename);
    if(kernel_dims_original == NULL) {
        return -1;
    }
    int kernel_num_dims = kernel_dims_original[0];
    int *kernel_dims = kernel_dims_original+1;
    float *kernel_data = read_array(B_filename, kernel_dims, kernel_num_dims);
    if(kernel_data == NULL) {
        return -1;
    }

    long int total_input_size = input_dims[0] * kernel_dims[1];
    
    float *output = malloc(sizeof(float) * total_input_size);


    op2(input_data, input_dims[0], input_dims[1], kernel_dims[1], kernel_data, output);


    if(compareOutput) {
        int *output_dims_original = read_dims(C_filename);
        if(output_dims_original == NULL) {
            return -1;
        }
        int output_num_dims = output_dims_original[0];
        int *output_dims = output_dims_original+1;
        float *expected_output = read_array(C_filename, output_dims, output_num_dims);

        if(expected_output == NULL) {
            return -1;
        }

        for(int i=0;i<total_input_size; i++) {
            if(fabs(output[i]-expected_output[i])>0.000001) {
                match = false;
                printf("At position %d, expected %f but found %f. \n", i, expected_output[i], output[i]);
                break;
            }
        }

        free(output_dims_original);
        free(expected_output);
    } else {
        FILE *file = fopen(C_filename,"w");

        if(file == NULL) {
            printf("Unable to open file: %s", C_filename);
            return -1;
        }

        if (file != NULL) {
            for(int i=0; i<input_num_dims; i++) {
                fprintf(file, "%d ", input_dims[i]);
            }
            fprintf(file, "\n");
        }

        for(int i=0; i<total_input_size; i++) {
            fprintf(file, "%.6f ", output[i]);
        }

        fclose(file);
    }



 /*

    //bool tc1 = test_case1();

    //printf("Test case 1: %s\n", tc1?"T":"F");

    //return
    */
}



bool test_case1() {
    int m = 3;
    int n = 3;
    int p = 3;

    float a_def[9] = {1,2,3,    4,5,6,    7,8,9};
    float b_def[9] = {2,3,4,   5,6,7,     8,9,10};
    float c_expected[9] = {36,42,48,    81,96,111,    126,150,174};

    float *a = malloc(sizeof(float) * m * n);
    float *b = malloc(sizeof(float) * n * p);
    float *c = malloc(sizeof(float) * m * p);

    for(int i=0;i<(m*n); i++) {
        a[i] = a_def[i];
    }

    for(int i=0;i<(n*p); i++) {
        b[i] = b_def[i];
    }

    for(int i=0;i<(m*p); i++) {
        c[i] = 0;
    }

    op2(a, m, n, p, b, c);

    bool match = true;

    for(int i=0;i<(m*p); i++) {
        if(c[i]!=c_expected[i]) {
            match = false;
            printf("At position %d, expected %f but found %f. \n", i, c_expected[i], c[i]);
            //break;
        }
    }
    free(a);
    free(b);
    free(c);
    return match;
}