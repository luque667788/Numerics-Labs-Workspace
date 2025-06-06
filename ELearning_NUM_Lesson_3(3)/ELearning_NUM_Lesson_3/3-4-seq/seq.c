#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#define SEQ_MAX		100

struct SEQ
{
	double x[SEQ_MAX+1];
	int n;
};

struct SEQ seq;

void calc(int n)
{
	if (n > SEQ_MAX)
		return;
	
	seq.n = 1;

	seq.x[seq.n] = 0.0;

	while (seq.n < n+1)
	{
		seq.x[seq.n] = pow((-1.0), (seq.n + 1.0))/(seq.n);

		(seq.n)++;
	}
}

void init(void)
{
	int n;

	for (n=0; n < SEQ_MAX; n++)
		seq.x[n] = 0.0;
	
	seq.n = 0;
}

int main(void)
{
	init();

	calc(10);

	int n;

	for (n=1; n < seq.n; n++)
	{
		printf("x(%2d) = % 1.2lf\n", n, seq.x[n]);
	}

    return EXIT_SUCCESS;
}
