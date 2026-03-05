/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_rgb.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 13:56:02 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 14:55:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"

// // Accepts the format 205,50,200,100
// // If only 3 values, it will assume 00RRGGBB
// static
// uint8_t	stt_get_channel(const char **str_ptr)
// {
// 	size_t		i;
// 	const char	*str = *str_ptr;
// 	uint8_t		number;

// 	number = 0;
// 	i = 0;
// 	while (ft_isspace(*str))
// 		str++;
// 	while (str[i] >= '0' && str[i] <= '9')
// 	{
// 		number = number * 10 + (str[i] - '0');	
// 		i++;
// 	}
// 	if (i == 0 || i > 3)
// 	{
// 		*str_ptr = NULL;
// 		return (0);
// 	}
// 	*str_ptr = str + i;
// 	return (number);
// }

// static
// char	*validate_rgb(const char *str)
// {
// 	size_t		i;
// 	size_t		count;

// 	count = 0;
// 	while (ft_isspace(*str))
// 		str++;
// 	while (((*str >= '0' && *str <= '9') || *str == ',') && count < 4)
// 	{
// 		if (*str == ',')
// 		{
// 			i = 1;
// 			while (str[i] >= '0' && str[i] <= '9')
// 				i++;
// 			if (i == 1 || i > 4)
// 				return (NULL);
// 			count++;
// 		}
// 		str++;
// 	}
// 	return (str);
// }

// TODO: Valid candidate for variadic
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

uint32_t	ft_strtoargb(const char *str, const char **str_ptr)
{
	size_t		i;
	uint32_t	argb_value;
	int64_t		number;

	i = 0;
	argb_value = 0;
	while (i < 4)
	{
		number = ft_strtol(str, &str);
		if (number < 0 || number > 255 || (i < 3 && *str++ != ','))	// Optionally could just cast to char and read any value
		{
			if (str_ptr != NULL)
				*str_ptr = NULL;
			return (0);
		}
		argb_value = (argb_value << 8) + (uint32_t) number;
		i++;
	}
	if (str_ptr != NULL)
		*str_ptr = str;
	return (argb_value);
}
