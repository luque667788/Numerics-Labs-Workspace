#include <stdio.h>
#include <stdlib.h>

static int factorial(int n)
{
    int res = 1;
    int k;
    for (k=2; k<=n; k++)
        res *= k;
    return res;
}

static int binomial(int n, int k)
{
    return factorial(n) / factorial(k) / factorial(n-k);
}

// binomial(15, 8) = 6435

int main(void)
{
    int n = 15;
    int k = 8;

    printf("(%i %i) = %i\n", n, k, binomial(n, k));

    return EXIT_SUCCESS;
}
