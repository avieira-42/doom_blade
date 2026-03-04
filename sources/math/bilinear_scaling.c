/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_scaling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/04 18:01:10 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "tmp_math.h"

static inline
uint32_t	stt_lerp_argb(uint32_t p0, uint32_t p1, uint8_t alpha)
{
	uint32_t	rb;
	uint32_t	ga;

	rb = p0 & 0x00FF00FF;									// Masks red and blue channel
	ga = (p0 >> 8) & 0x00FF00FF;							// Shifts green and alpha channel then masks
	rb += (((p1 & 0x00FF00FF) - rb) * alpha) >> 8;			// 
	ga += ((((p1 >> 8) & 0x00FF00FF) - ga) * alpha) >> 8;
	return (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
}

static
uint32_t	stt_bilerp(t_mat2 sample, float u, float v)
{
	uint32_t	row1;
	uint32_t	row2;
	uint32_t	result;

	row1 = stt_lerp_argb(sample.p00, sample.p01, u * 255);
	row2 = stt_lerp_argb(sample.p10, sample.p11, u * 255);
	result = stt_lerp_argb(row1, row2, v * 255);
	return (result);
}

static
uint32_t	stt_lerp(t_mat32 *src, t_vec2 delta_src, t_vec2 pos)
{
	float	u;
	float	v;
	t_mat2	sample;
	t_mat2	index;

	index.p00 = src->cols * pos.x.f;
	index.p01 = src->cols * pos.x.f + (index.p00 < src->cols);
	index.p10 = src->rows * pos.y.f;
	index.p11 = src->rows * pos.y.f + (index.p10 < src->rows);
	sample.p00 = src->ptr[index.p10 * src->cols + index.p00];
	sample.p01 = src->ptr[index.p10 * src->cols + index.p01];
	sample.p10 = src->ptr[index.p11 * src->cols + index.p00];
	sample.p11 = src->ptr[index.p11 * src->cols + index.p01];
	u = (pos.x.f - index.p00 * delta_src.x.f) / delta_src.x.f;	// index.p00 * dx gets the clamped float coordinate
	v = (pos.y.f - index.p10 * delta_src.y.f) / delta_src.y.f;
	return (stt_bilerp(sample, u, v));
}

// Assumption is scale to fit
void	ft_bilinear_scaling(t_mat32 *src, t_mat32 *dst)
{
	uint32_t		x;
	uint32_t		y;
	const t_vec2	delta_src = {{1.0f / (double)src->cols}, {1.0f / (double)src->rows}};
	const t_vec2	delta_dst = {{1.0f / (double)dst->cols}, {1.0f / (double)dst->rows}};
	t_vec2			pos;

	y = 0;
	while (y < dst->rows)
	{
		x = 0;
		pos.x.f = 0.0f;
		pos.y.f = y * delta_dst.y.f;
		while (x < dst->cols)
		{
			dst->ptr[y * dst->cols + x] = stt_lerp(src, delta_src, pos);
			pos.x.f += delta_dst.x.f;
			x++;
		}
		y++;
	}
}
