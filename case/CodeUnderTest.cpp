#include "includes.h"

int main(){
    const size_t ELEMS = 1000*1000;
    float starting_vec[ELEMS];
    initialise(starting_vec, ELEMS);
    
    for (size_t i = 0; i < ELEMS; ++i){
        starting_vec[i] /= 2;
        starting_vec[i] += 4;
    }
    
    // Calculate mean
    float k = 0;
    for (size_t i = 0; i < ELEMS; ++i)
        k += starting_vec[i]/ELEMS;

    // Renormalise all values and compute cumulative sum
    float cum_sum[ELEMS];
    cum_sum[0] = starting_vec[0]/k;
    for (size_t i = 1; i < ELEMS; i++)
        cum_sum[i] = starting_vec[i]/ELEMS + cum_sum[i-1];
    /* ... */
}

void mmult(float **A, float **B, float **C, size_t dim){
    for (size_t i = 0; i < dim; ++i)
        for (size_t j = 0; j < dim; ++j)
            for (size_t k = 0; k < dim; ++k)
                C[i][j] += A[i][k] * B[k][j];
}
