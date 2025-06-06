#include <stdio.h>
#include <stdlib.h>

static long long factorial(int n)
{
    long long res = 1;
    int k;
    for (k=2; k<=n; k++)
        res *= k;
    return res;
}

static long long binomial(int n, int k)
{
    long long a = factorial(n);
    long long b = factorial(k);
    long long c = factorial(n-k);
    long long res = a / b / c;
    return res;
}

// binomial(15, 8) = 6435

int main(void)
{
    int n = 15;
    int k = 8;

    printf("(%i %i) = %i\n", n, k, binomial(n, k));

    return EXIT_SUCCESS;
}
