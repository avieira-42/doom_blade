/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_scaling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/19 15:02:30 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"

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

static inline
uint32_t	stt_bilerp(const t_mat32 *src, t_mat2 index, t_vec2 src_pos)
{
	t_mat2		sample;
	uint32_t	row1;
	uint32_t	row2;
	uint32_t	result;

	sample.p00 = src->ptr[index.p10 * src->cols + index.p00];
	sample.p01 = src->ptr[index.p10 * src->cols + index.p01];
	sample.p10 = src->ptr[index.p11 * src->cols + index.p00];
	sample.p11 = src->ptr[index.p11 * src->cols + index.p01];
	row1 = stt_lerp_argb(sample.p00, sample.p01, src_pos.x.f * 256);
	row2 = stt_lerp_argb(sample.p10, sample.p11, src_pos.x.f * 256);
	result = stt_lerp_argb(row1, row2, src_pos.y.f * 256);
	return (result);
}

// TODO: Assumption used to be scale to fit, review algorithm accordingly and add pos draw
void	ft_bilinear_scaling(const t_mat32 *src, t_mat32 *dst, t_vec2 scale, t_vec2 pos)
{
	uint32_t		x;
	uint32_t		y;
	t_vec2			src_pos;
	t_mat2			index;

	y = 0;
	while (y < dst->rows)
	{
		x = 0;
		src_pos.y.f = fmaxf(0.0f, scale.y.f * ((float)y + 0.5f) - 0.5f);
		index.p10 = src_pos.y.f;
		index.p11 = src_pos.y.f + (index.p10 < (src->rows - 1));
		src_pos.y.f = src_pos.y.f - (float) index.p10;	// Normalizes to 0-1, v coordinate
		while (x < dst->cols)
		{
			src_pos.x.f = fmaxf(0.0f, scale.x.f * ((float)x + 0.5f) - 0.5f);
			index.p00 = src_pos.x.f;
			index.p01 = src_pos.x.f + (index.p00 < (src->cols - 1));
			src_pos.x.f = src_pos.x.f - (float) index.p00;		// Normalizes to 0-1, u coordinate
			dst->ptr[y * dst->cols + x] = stt_bilerp(src, index, src_pos);
			x++;
		}
		y++;
	}
}
