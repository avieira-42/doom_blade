/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   convert.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/15 15:50:30 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/22 15:30:57 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <threads.h>

//size_t		ft_strlen(const char *str);
void		*ft_memset(void *vdst, const uint8_t byte, size_t length);

// To do: set errno on overflow
/*int64_t	ft_strtol(const char *str)
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
	return (sign * number);
}*/

int64_t	ft_strntol(const char *str, size_t length)
{
	const char	*end = str + length;
	int64_t		number;
	int64_t		sign;

	number = 0;
	while (str < end && (*str == ' ' || (*str >= '\t' && *str <= '\r')))
		str++;
	sign = -1 + ((*str == '-') << 1);
	str += (sign == 1) || (*str == '+');
	while (*str >= '0' && *str <= '9' && str < end)
		number = number * 10 - (*str++ - '0');
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
