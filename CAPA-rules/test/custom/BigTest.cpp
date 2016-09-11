#include <iostream>
#define SIZE 1000000


int main()
{
    float returns[SIZE];
    float cumulative[SIZE] = {0};
    for (int i = 0; i < SIZE; ++i)
    {
        // Returns are in percentage terms, so change them down to actual percentages
        returns[i] = returns[i] / 100;
        // Calculate the incremental returns per unit time.
        cumulative[i] = returns[i];
        if (i)
        {
            cumulative[i] += cumulative[i-1];
        }
    }
    return 0;
}
