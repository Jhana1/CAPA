typedef unsigned long long size_t;
///CAPA:IGNORE
int main()
{
    const int r = 10;
    int a[r][r], b[r][r], c[r][r];
    for (int row = 0; row < r; ++row)
    {
        for (int col = 0; col < r; ++col)
        {
            for (int inner = 0; inner < r; ++inner)
            {
                c[row][col] += a[row][inner] * b[inner][col];
            }
        }
    }    
}

/**CAPA:IGNORE*/
void mmult(float **A, float **B, float **C, size_t dim)
{
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++i)
            for (int k = 0; k < dim; ++i)
                C[i][j] += A[i][k] * B[k][j];
}


