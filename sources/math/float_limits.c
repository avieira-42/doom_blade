/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_limits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:06:26 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/28 16:48:30 by adeimlin         ###   ########.fr       */
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
