#include <stdint.h>

int64_t	ft_imin(int64_t number1, int64_t number2)
{
	if (number1 <= number2)
		return (number1);
	else
		return (number2);
}

int64_t	ft_imax(int64_t number1, int64_t number2)
{
	if (number1 >= number2)
		return (number1);
	else
		return (number2);
}

int64_t	ft_iclamp(int64_t value, int64_t min, int64_t max)
{
	if (value > max)
		value = max;
	else if (value < min)
		value = min;
	return (value);
}
