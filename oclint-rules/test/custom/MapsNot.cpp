int patterns()
{
    int *a, *b, *c;
    int i, j, k;
    
    for (i = 0; i < j; i++)
    // not a map
    {
        a[i] = a[j-i];
    }

    for (i = 0; i < j; ++i)
    // not a map
    // is a running-average
    {
        a[i] = a[i] * a[i-1];
    }

    for (i = 0; i < j; ++i)
    // Not a map
    // TODO: NEEDS HANDLING STILL!
    {
        a[i] = a[i] * b[i];
    }
    return 0;
}
