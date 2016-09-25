int f(int j)
{
    return j * 2;
}

/// CAPA:IGNORE
int main() // More Comments
{
    int *a, *b, *c;
    int i, k;

    const int j = 100000000;
    for (i = 1; i < j; ++i)
    {
        a[i] += a[i-1];
    }

    for (i = 0; i < j; ++i)
    {
        a[i] = a[i-1] + a[i];
    }

    for (i = 0; i < j; ++i)
    {
        a[i] = a[i] + a[i-1];
    }

    for (i = 0; i < 1; ++i)
    {
        c[i] = f(c[i-1]);
    }

    return 0;
}
