#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

void random_fill(float *starting_vec, size_t size)
{
    for (size_t i = 0; i < size; ++i)
        starting_vec[i] = (float) rand(); 
}

void reshape2mat(float *in_vec, float *out_vec[], size_t dim)
{
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            out_vec[i][j] = in_vec[i*dim + j];
        }
    }
}

void reshape2vec(float *out_vec, float *in_mat[], size_t dim)
{
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++j)
        {
            out_vec[i*dim + j] = in_mat[i][j];
        }
    }
}

void mmult(float **A, float **B, float **C, size_t dim)
{
    for (size_t i = 0; i < dim; ++i)
    {
        for (size_t j = 0; j < dim; ++i)
        {
            for (size_t k = 0; k < dim; ++i)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main()
{
    // Toy Example for Test example Case.
    const size_t ELEMS = 1000*1000;
    float starting_vec[ELEMS];
    time_t t;
    
    srand((unsigned) time(&t));
    random_fill(starting_vec, ELEMS);
    
    // Divide all points by 2 and add 4 for later stage
    for (size_t i = 0; i < ELEMS; ++i)
    {
        starting_vec[i] /= 2;
        starting_vec[i] += 4;
    }
    
    // Calculate mean
    float k = 0;
    for (size_t i = 0; i < ELEMS; ++i)
    {
        k += starting_vec[i]/ELEMS;
    }

    // Renormalise all values and compute cumulative sum
    float cum_sum[ELEMS];
    cum_sum[0] = starting_vec[0]/k;
    for (size_t i = 1; i < ELEMS; i++)
    {
        cum_sum[i] = starting_vec[i]/ELEMS + cum_sum[i-1];
    }

    // Prepare for and perform Matrix Mult
    const size_t dim = sqrt(ELEMS);
    float cum_sum_mat[dim][dim];

    reshape2mat(cum_sum, (float **) cum_sum_mat, dim);
    mmult((float **) cum_sum_mat, (float **) cum_sum_mat, (float **) cum_sum_mat, dim);
    reshape2vec(cum_sum, (float **) cum_sum_mat, dim);

    // reduce only even values, but do it with conditional
    // and deliberately prevent it being caught
    k = 0;
    for (size_t i = 0; i < ELEMS; ++i)
    {
        if (!(i % 2))
            k = k + cum_sum[i + 1 - 1];
    }
    return k;
}
