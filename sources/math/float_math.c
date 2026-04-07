/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_math.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:06:26 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 17:06:15 by adeimlin         ###   ########.fr       */
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

float	ft_clamp(float value, float min, float max)
{
	if (value > max)
		value = max;
	else if (value < min)
		value = min;
	return (value);
}

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
