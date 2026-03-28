/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_limits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:06:26 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/28 12:38:37 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cmlx_base.h"

float	ft_min(float number1, float number2)
{
	if (number1 <= number2)
		return (number1);
	else
		return (number2);
}

float	ft_max(float number1, float number2)
{
	if (number1 >= number2)
		return (number1);
	else
		return (number2);
}

float	ft_abs(float number)
{
	t_32 number_union;

	number_union.f = number;
	number_union.u &= 0x7FFFFFFF;
	return (number_union.f);
}

float	ft_absdiff(float number1, float number2)
{
	t_32	result;

	result.f = number1 - number2;
	result.u &= 0x7FFFFFFF;
	return (result.f);
}

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
