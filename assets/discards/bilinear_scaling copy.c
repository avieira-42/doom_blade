/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_scaling copy.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/21 14:56:00 by adeimlin         ###   ########.fr       */
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
uint32_t	stt_bilerp(const t_mat32 *src, t_vec2 scale, t_vec2 dst_pos)
{
	uint32_t	p0;
	uint32_t	p1;
	uint32_t	row1;
	uint32_t	row2;
	uint32_t	result;
	t_vec2		src_pos;
	t_vec2		col0;
	t_vec2		col1;

	src_pos.y.f = fmaxf(0.0f, scale.y.f * ((float)dst_pos.y.u + 0.5f) - 0.5f);
	col0.y.u = src_pos.y.f;
	col1.y.u = src_pos.y.f + (col0.y.u < (src->rows - 1));
	src_pos.y.f = src_pos.y.f - (float) col0.y.u;	// Normalizes to 0-1, v coordinate
	src_pos.x.f = fmaxf(0.0f, scale.x.f * ((float)dst_pos.x.u + 0.5f) - 0.5f);
	col0.x.u = src_pos.x.f;
	col1.x.u = src_pos.x.f + (col0.x.u < (src->cols - 1));
	src_pos.x.f = src_pos.x.f - (float) col0.x.u;		// Normalizes to 0-1, u coordinate

	p0 = src->ptr[col0.y.u * src->cols + col0.x.u];
	p1 = src->ptr[col0.y.u * src->cols + col1.x.u];
	row1 = stt_lerp_argb(p0, p1, src_pos.x.f * 256);
	p0 = src->ptr[col1.y.u * src->cols + col0.x.u];
	p1 = src->ptr[col1.y.u * src->cols + col1.x.u];
	row2 = stt_lerp_argb(p0, p1, src_pos.x.f * 256);
	result = stt_lerp_argb(row1, row2, src_pos.y.f * 256);
	return (result);
}

// TODO: Assumption used to be scale to fit, review algorithm accordingly and add pos draw
void	ft_bilinear_scaling(const t_mat32 *src, t_mat32 *dst, t_vec2 scale, t_vec2 pos)
{
	uint32_t		x;
	uint32_t		y;
	t_vec2			src_pos;
	t_vec2			col0;
	t_vec2			col1;

	y = 0;
	while (y < dst->rows)
	{
		x = 0;
		src_pos.y.f = fmaxf(0.0f, scale.y.f * ((float)y + 0.5f) - 0.5f);
		col0.y.u = src_pos.y.f;
		col1.y.u = src_pos.y.f + (col0.y.u < (src->rows - 1));
		src_pos.y.f = src_pos.y.f - (float) col0.y.u;	// Normalizes to 0-1, v coordinate
		while (x < dst->cols)
		{
			src_pos.x.f = fmaxf(0.0f, scale.x.f * ((float)x + 0.5f) - 0.5f);
			col0.x.u = src_pos.x.f;
			col1.x.u = src_pos.x.f + (col0.x.u < (src->cols - 1));
			src_pos.x.f = src_pos.x.f - (float) col0.x.u;		// Normalizes to 0-1, u coordinate
			dst->ptr[y * dst->cols + x] = stt_bilerp(src, col0, col1, src_pos);
			x++;
		}
		y++;
	}
}
