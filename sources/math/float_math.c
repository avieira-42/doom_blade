#include "game_types.h"

float	ft_qinvsqrt(float number)
{
	t_32	result;

	result.f = number;
	result.u = 0x5f3759df - (result.u >> 1);
	result.f = result.f * (1.5f - (number * 0.5f * result.f * result.f));
	return (result.f);
}

float	ft_qsqrt(float number)
{
	t_32	result;

	result.f = number;
	result.u = (result.u >> 1) + 0x1FBD3EE0;
	result.f = 0.5f * (result.f + number / result.f);
	return (result.f);
}

// float	ft_absclamp(float number, float value)
// {
// 	t_32		number_union;
// 	t_32		value_union;
// 	uint32_t	sign_mask;

// 	number_union.f = number;
// 	value_union.f = value;
// 	sign_mask = (number_union.u & 0x80000000);
// 	number_union.u &= 0x7FFFFFFF;
// 	value_union.u &= 0x7FFFFFFF;
// 	if (number_union.f >= value_union.f)
// 	{
// 		value_union.u |= sign_mask;
// 		return (value_union.f);
// 	}
// 	else
// 		return (number);
// }