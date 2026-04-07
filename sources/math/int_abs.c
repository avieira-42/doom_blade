/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   int_abs.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 20:31:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 20:31:38 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>

int64_t	ft_iabs(int64_t number)
{
	if (number >= 0)
		return (number);
	else
		return (-number);
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

int64_t	ft_iabsdiff(int64_t number1, int64_t number2)
{
	if (number1 >= number2)
		return (number1 - number2);
	else
		return (number2 - number1);
}
