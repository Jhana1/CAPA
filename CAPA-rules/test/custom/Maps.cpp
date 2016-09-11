#include <stdlib.h>

int someFunc(...)
{
   return 3;
}

int someFunc2(int i, int *a)
{
    return 3;
}

int patterns()
{
    int *a, *b, *c;
    int i, j, k;
    

    const int jj = 100;
    
    
    for (int i = 0; i < j; ++i)
    // map init
    {
        a[i] = b[i] * 2;
    }

    for (i = 0; i < j; ++i)
    // map no init
    {
        a[i] = b[i] * 2;
    }

    for (i = 0; i < j; ++i)
    // map self referential
    {
        a[i] = a[i] * 2;
    }

    for (i = 0; i <= j; i++)
    // map odd
    {
        a[i] = 2 + 3 + 4 * 8 * a[i]; 
    }
    
    for (i = 0; i < j; i++)
    // not a map
    // well actually it is a map, but it's just a memset...
    {
        a[i] = a[j];
    }

    for (i = 1; i < j; ++i)
    // non-zero map
    {
        a[i] = b[i] + 2;
    }

    for (i = 0; i < j; ++i)
    {
        a[i] = someFunc(a[i],1,2,3);
    }
    
    for (i = 0; i < j; ++i)
    {
        int j = 3 + 3;
        b[j] = 23;
        a[i] = b[i] + 2;
    }
    for (i = 0; i < j; i += jj)
    {
        a[i] = b[i];
    }
    while (i++ < 10)
    {
        a[i] = b[i] + 1;
    }
    return 0;
}
