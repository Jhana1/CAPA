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
