#include "cmlx_base.h"

float	ft_abs(float number)
{
	t_32	number_union;

	number_union.f = number;
	number_union.u &= 0x7FFFFFFF;
	return (number_union.f);
}

// float	ft_absdiff(float number1, float number2)
// {
// 	t_32	result;

// 	result.f = number1 - number2;
// 	result.u &= 0x7FFFFFFF;
// 	return (result.f);
// }

// float	ft_abssum(float number1, float number2)
// {
// 	t_32	result;

// 	result.f = number1 - number2;
// 	result.u &= 0x7FFFFFFF;
// 	return (result.f);
// }

float	ft_absclamp(float number, float value)
{
	t_32		number_union;
	t_32		value_union;
	uint32_t	sign_mask;

	number_union.f = number;
	value_union.f = value;
	sign_mask = (number_union.u & 0x80000000);
	number_union.u &= 0x7FFFFFFF;
	value_union.u &= 0x7FFFFFFF;
	if (number_union.f >= value_union.f)
	{
		value_union.u |= sign_mask;
		return (value_union.f);
	}
	else
		return (number);
}
