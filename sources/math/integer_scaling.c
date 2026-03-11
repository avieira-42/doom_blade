/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer_scaling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 15:23:58 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/11 14:41:47 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"
#include "cub_structs.h"

static inline
void	stt_linecpy(uint32_t *restrict dst, uint32_t *restrict src, size_t factor, size_t width)
{
	size_t			i;
	const uint32_t	*src_end = src + width;
	uint32_t		tmp;

	while (src < src_end)
	{
		i = 0;
		tmp = *src;
		while (i < factor)
		{
			*dst++ = tmp;
			i++;
		}
		src++;
	}
}

// Do we do error checking here? (opted not to, makes more sense to do it from outside)
// Assumption is image is stored in width by height
void	ft_integer_scaling(t_mat32 src, t_mat32 dst, size_t factor)
{
	size_t			i;
	uint32_t		*src_end;
	uint32_t		*odst;
	const size_t	line_size = src.cols * factor * sizeof(uint32_t);

	src_end = src.ptr + src.cols * src.rows;
	while (src.ptr < src_end)
	{
		odst = dst.ptr;
		stt_linecpy(dst.ptr, src.ptr, factor, src.cols);	// Copies the line, 123 becomes 112233
		dst.ptr += dst.cols;
		i = 1;
		while (i < factor)
		{
			ft_memcpy(dst.ptr, odst, line_size);	// Copies the line across N rows
			dst.ptr += dst.cols;
			i++;
		}
		src.ptr += src.cols;
	}
}

static inline
void	stt_linecpy_t(uint32_t *restrict dst, t_mat32 src, size_t factor)
{
	size_t			i;
	const uint32_t	*src_end = src.ptr + src.cols * src.rows;
	uint32_t		tmp;

	while (src.ptr < src_end)
	{
		i = 0;
		tmp = *src.ptr;
		while (i < factor)
		{
			*dst++ = tmp;
			i++;
		}
		src.ptr += src.cols;
	}
}

void	ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor)
{
	size_t			i;
	uint32_t		*src_end;
	uint32_t		*odst;
	const size_t	line_size = src.rows * factor * sizeof(uint32_t);

	src_end = src.ptr + src.cols;
	while (src.ptr < src_end)
	{
		odst = dst.ptr;
		stt_linecpy_t(dst.ptr, src, factor);	// Copies the line, 123 becomes 112233
		dst.ptr += dst.cols;
		i = 1;
		while (i < factor)
		{
			ft_memcpy(dst.ptr, odst, line_size);	// Copies the line across N rows
			dst.ptr += dst.cols;
			i++;
		}
		src.ptr += 1;
	}
}
