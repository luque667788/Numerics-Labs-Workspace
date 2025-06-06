#include <stdio.h>
#include <stdlib.h>

int main(void)
{
	float x = -1.5;
	int n = 1;
	
	while (n < 10)
	{
		printf("x(%2d) = % 1.2f\n", n, x);

		x = x*x - 1.0;
		
		n++;
	}

    return EXIT_SUCCESS;
}
