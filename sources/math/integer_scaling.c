/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   integer_scaling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/28 15:23:58 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/08 16:12:01 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"
#include "cub_structs.h"

static inline
void	stt_linecpy(uint32_t *restrict src, uint32_t *restrict dst, size_t factor, size_t width)
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
void	ft_integer_scaling(t_mat32 *src, t_mat32 *dst, size_t factor)
{
	size_t			i;
	uint32_t		*dst_ptr;
	uint32_t		*src_ptr;
	uint32_t		*src_end;
	const size_t	line_size = dst->cols * sizeof(uint32_t);

	src_ptr = src->ptr;
	dst_ptr = dst->ptr;
	src_end = src->ptr + src->cols * src->rows;
	while (src_ptr < src_end)
	{
		stt_linecpy(src_ptr, dst_ptr, factor, src->cols);	// Copies the line, 123 becomes 112233
		i = 1;
		while (i < factor)
		{
			ft_memcpy(dst_ptr + dst->cols, dst_ptr, line_size);	// Copies the line across N rows
			dst_ptr += dst->cols;
			i++;
		}
		src_ptr += src->cols;
	}
}

// TODO: Transposed version
void	ft_integer_scaling_t(t_mat32 *src, t_mat32 *dst, size_t factor)
{
	size_t			i;
	uint32_t		*dst_ptr;
	uint32_t		*src_ptr;
	uint32_t		*src_end;
	const size_t	line_size = dst->cols * sizeof(uint32_t);

	src_ptr = src->ptr;
	dst_ptr = dst->ptr;
	src_end = src->ptr + src->cols * src->rows;
	while (src_ptr < src_end)
	{
		stt_linecpy(src_ptr, dst_ptr, factor, src->cols);	// Copies the line, 123 becomes 112233
		i = 1;
		while (i < factor)
		{
			ft_memcpy(dst_ptr + dst->cols, dst_ptr, line_size);	// Copies the line across N rows
			dst_ptr += dst->cols;
			i++;
		}
		src_ptr += src->cols;
	}
}
