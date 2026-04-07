/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:50:30 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 17:18:22 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <threads.h>

size_t	ft_strlen(const char *str)
{
	const char	*ostr = str;

	while (*str != 0)
		str++;
	return ((size_t)(str - ostr));
}

// To do: set errno on overflow
int64_t	ft_strtol(const char *str, const char **str_ptr)
{
	int64_t	number;
	int64_t	sign;

	number = 0;
	while (*str == ' ' || (*str >= '\t' && *str <= '\r'))
		str++;
	sign = -1 + ((*str == '-') << 1);
	str += (sign == 1) || (*str == '+');
	while (*str >= '0' && *str <= '9')
		number = number * 10 - (*str++ - '0');
	if (str_ptr != NULL)
		*str_ptr = str;
	return (sign * number);
}

// This function fills a buffer with up to 24 ascii digits and null terminates
// Returns the length of the number
size_t	ft_itoa_r(int64_t number, char *ptr)
{
	char			buffer[24];
	char			*wptr;
	const int8_t	sign = (number >= 0) - (number < 0);
	const char		*optr = ptr;
	size_t			length;

	wptr = buffer + sizeof(buffer) - 1;
	*wptr = 0;
	*(--wptr) = sign * (number % 10) + '0';
	number = sign * (number / 10);
	while (number != 0)
	{
		*(--wptr) = (number % 10) + '0';
		number /= 10;
	}
	if (sign == -1)
		*(--wptr) = '-';
	length = (size_t)((buffer + sizeof(buffer)) - wptr); // Copies \0
	while (length > 0)
	{
		*ptr++ = *wptr++;
		length--;
	}
	return ((size_t)(ptr - optr) - 1);
}

uint32_t	ft_strtoargb(const char *str, const char **str_ptr)
{
	size_t		i;
	uint32_t	argb_value;
	int64_t		number;

	i = 0;
	argb_value = 0;
	while (i < 4)
	{
		number = (uint8_t)ft_strtol(str, &str);
		argb_value = (argb_value << 8) + (uint32_t) number;
		i++;
		if (*str != ',')
			break ;
		str++;
	}
	if (i < 3)
		str = NULL;
	if (str_ptr != NULL)
		*str_ptr = str;
	return (argb_value);
}
