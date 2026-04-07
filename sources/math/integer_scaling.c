/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer_scaling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:29:31 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 15:29:32 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"
#include "cub_structs.h"

static inline
void	stt_linecpy_t(uint32_t *restrict dst, const t_mat32 src, size_t factor)
{
	size_t			i;
	size_t			k;
	uint32_t		tmp;
	const size_t	length = src.height * src.width;

	i = 0;
	while (i < length)
	{
		k = 0;
		tmp = src.ptr[i];
		while (k < factor)
		{
			*dst++ = tmp;
			k++;
		}
		i += src.height;
	}
}

// TODO: Add pos start for draw on DST, assumption isnt scale to fit
void	ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor)
{
	size_t			k;
	uint32_t		*src_end;
	uint32_t		*odst;
	const size_t	line_size = src.width * factor * sizeof(uint32_t);

	src_end = src.ptr + src.height;
	while (src.ptr < src_end)
	{
		odst = dst.ptr;
		stt_linecpy_t(dst.ptr, src, factor);	// Copies the line, 123 becomes 112233
		dst.ptr += dst.stride;
		k = 1;
		while (k < factor)
		{
			ft_memcpy(dst.ptr, odst, line_size);	// Copies the line across N rows
			dst.ptr += dst.stride;
			k++;
		}
		src.ptr += 1;
	}
}
