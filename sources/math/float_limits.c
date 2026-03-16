/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_limits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:06:26 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/16 13:21:25 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_types.h"

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
	t_32 value;

	value.f = number;
	value.u &= 0x7FFFFFFF;
	return (value.f);
}

float	ft_absdiff(float number1, float number2)
{
	if (number1 >= number2)
		return (number1 - number2);
	else
		return (number2 - number1);
}

float	ft_symclamp(float number, float value)
{
	t_32		a;
	t_32		b;
	uint32_t	sign_mask;

	a.f = number;
	b.f = value;
	sign_mask = (a.u & 0x80000000);
	a.u = (a.u & 0x7FFFFFFF);
	b.u = (b.u & 0x7FFFFFFF);
	if (a.f >= b.f)
	{
		b.u |= sign_mask;
		return (b.f);
	}
	else
		return (number);
}
