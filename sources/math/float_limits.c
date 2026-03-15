/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_limits.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/22 12:06:26 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/15 13:06:06 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (number >= 0.0f)
		return (number);
	else
		return (-number);
}

float	ft_absdiff(float number1, float number2)
{
	if (number1 >= number2)
		return (number1 - number2);
	else
		return (number2 - number1);
}

float	ft_clamp(float number, float value)
{
	if (number >= value)
		return (value);
	else if (number <= -value)
		return (-value);
	return (number);
}
