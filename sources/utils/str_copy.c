/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_copy.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/12 20:25:16 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/17 11:43:43 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <threads.h>

size_t	ft_strlen(const char *str);

char	*ft_strcpy(char *dst, const char *src)
{
	const char	*osrc = src;
	size_t		length;
	char		*odst;

	while (*src != 0)
		src++;
	length = (size_t)(src - osrc + 1);
	odst = dst;
	while (length > 0)
	{
		*dst++ = *osrc++;
		length--;
	}
	return (odst);
}

char	*ft_strncpy(char *dst, const char *src, size_t length)
{
	char	*odst;

	odst = dst;
	while (*src != 0 && length > 0)
	{
		*dst++ = *src++;
		length--;
	}
	while (length > 0)
	{
		*dst = 0;
		dst++;
		length--;
	}
	return (odst);
}

char	*ft_strlcpy(char *restrict dst, const char *restrict src, size_t dst_size)
{
	char		*odst;
	size_t		src_len;
	size_t		length;

	odst = dst;
	src_len = 0;
	while (src[src_len] != 0)
		src_len++;
	if (dst_size != 0)
	{
		if (dst_size > src_len)
			length = src_len;
		else
			length = dst_size - 1;
		while (length > 0)
		{
			*dst++ = *src++;
			length--;
		}
		*dst = 0;
	}
	return (odst);
}

char	*ft_strdup(const char *src)
{
	const char	*osrc = src;
	char		*str;

	while (*src != 0)
		src++;
	str = (char *) malloc((size_t)(src - osrc + 1));
	if (str == NULL)
		return (NULL);
	str += src - osrc;
	*str = 0;
	while (src > osrc)
		*--str = *--src;
	return (str);
}

// Copies a null terminated array of strings into a single buffer
// Returns: NULL on full copy, or the mutated vec array for incomplete copies
const char	**ft_strvcpy(
	char **restrict			wdst	/*mutable pointer to a buffer*/,
	const char **restrict	vec		/*null terminated array of cstrings*/,
	char *restrict			end		/*end = pointer to the end of the buffer*/)
{
	const char	*restrict	src;
	char		*restrict	dst;
	size_t					length;
	size_t					space_left;

	dst = (char *restrict)*wdst;
	while (*vec != NULL)
	{
		src = (const char *restrict)*vec++;
		length = ft_strlen(src) + (*vec == NULL);
		while (length > 0)
		{
			if (dst >= end)
				return (vec - 1);
			space_left = (size_t)(end - dst);
			if (length < space_left)
				space_left = length;
			length -= space_left;
			while (space_left-- > 0)
				*dst++ = *src++;
			*wdst = dst;
			vec[-1] = src;
		}
	}
	return (NULL);
}
