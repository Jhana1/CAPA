int f(int j)
{
    return j * 2;
}

int main()
{
    int *a, *b, *c;
    int i, j, k;

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

    for (i = 0; i < j; ++i)
    {
        c[i] = f(c[i-1]);
    }

    return 0;
}