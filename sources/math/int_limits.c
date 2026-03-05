/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_limits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/13 19:03:49 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/03 11:39:49 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int64_t	ft_iabs(int64_t number)
{
	if (number >= 0)
		return (number);
	else
		return (-number);
}

int64_t	ft_iabsdiff(int64_t number1, int64_t number2)
{
	if (number1 >= number2)
		return (number1 - number2);
	else
		return (number2 - number1);
}

int64_t	ft_iabsmax(int64_t number1, int64_t number2)
{
	if (number1 < 0)
		number1 = -number1;
	if (number2 < 0)
		number2 = -number2;
	if (number1 >= number2)
		return (number1);
	else
		return (number2);
}
