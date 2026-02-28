/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 16:40:16 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/03 10:59:46 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <fcntl.h>

void	*ft_bzero(void *dst_void, size_t length)
{
	uint8_t	*dst;

	dst = (uint8_t *) dst_void;
	while (((uintptr_t)dst & (sizeof(uintptr_t) - 1)) && length > 0)
	{
		*dst++ = 0u;
		length--;
	}
	while (length >= sizeof(uintptr_t))
	{
		*((uintptr_t *)dst) = 0UL;
		dst += sizeof(uintptr_t);
		length -= sizeof(uintptr_t);
	}
	while (length > 0)
	{
		*dst++ = 0u;
		length--;
	}
	return (dst_void);
}

uint8_t	*ft_setlut256(uint8_t *lut, const char *charset)
{
	ft_bzero(lut, 256);
	while (*charset != 0)
		lut[*(const uint8_t *) charset++] = 1;
	lut[0] = 0xFF;
	return (lut);
}

size_t	ft_count_tokens(const char *str, const uint8_t *lut, size_t *bytes)
{
	const uint8_t	*ustr = (const uint8_t *) str;
	size_t			bytes_local;
	size_t			count;

	bytes_local = 0;
	while (lut[*ustr] == 1)
		ustr++;
	count = *ustr != 0;
	while (*ustr != 0)
	{
		count += lut[ustr[0]] && !lut[ustr[1]];
		bytes_local += !lut[ustr[0]];
		ustr++;
	}
	if (bytes != NULL)
		*bytes = bytes_local;
	return (count);
}

ssize_t	ft_read_size(const char *filename)
{
	const int32_t	fd = open(filename, O_RDONLY);
	uint8_t			buffer[16 * 1024];
	ssize_t			bytes_read;
	ssize_t			bytes_total;

	if (fd < 0)
		return (-1);
	bytes_total = 0;
	bytes_read = read(fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		bytes_total += bytes_read;
		bytes_read = read(fd, buffer, sizeof(buffer));
	}
	if (bytes_read < 0)
		return (-1);
	close(fd);
	return (bytes_total);
}
