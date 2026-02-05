#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "code/libft_math.h"

int	main(int argc, char **argv)
{
	if (argc != 3)
		return (1);

	int			a = atoi(argv[0]);
	int			b = atoi(argv[1]);
	t_vec const x = {a, b};

	printf("sqrt: %f\n", sqrtf(a * a + b * b));
	printf("q_sqrt: %f\n", 1 / q_rsqrt(a * a + b * b));
	printf("u_sqrt: %f\n", 1 / u_rsqrt(a * a + b * b));
}
