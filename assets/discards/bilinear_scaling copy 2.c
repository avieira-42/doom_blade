/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_scaling copy 2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/21 16:14:18 by adeimlin         ###   ########.fr       */
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
uint32_t	stt_bilerp_argb(t_vec4 sample, float uu, float vv)
{
	const uint8_t	u = uu * 255;
	const uint8_t	v = vv * 255;
	uint32_t		row1;
	uint32_t		row2;
	uint32_t		result;

	row1 = stt_lerp_argb(sample.x.u, sample.y.u, u);
	row2 = stt_lerp_argb(sample.z.u, sample.w.u, u);
	result = stt_lerp_argb(row1, row2, v);
	return (result);
}

static inline
uint32_t	stt_bilerp(const t_mat32 *src, t_vec2 norm_pos)
{
	t_vec2		src_pos;
	float		u;
	float		v;
	t_vec4		index;
	t_vec4		sample;

	src_pos.x.f = norm_pos.x.f * (src->cols - 1);
	src_pos.y.f = norm_pos.y.f * (src->rows - 1);
	index.x.u = src_pos.y.f;
	index.y.u = src_pos.y.f + (index.x.u < (src->rows - 1));
	index.z.u = src_pos.x.f;
	index.w.u = src_pos.x.f + (index.z.u < (src->cols - 1));
	sample.x.u = src->ptr[index.x.u * src->cols + index.z.u];
	sample.y.u = src->ptr[index.x.u * src->cols + index.w.u];
	sample.z.u = src->ptr[index.y.u * src->cols + index.z.u];
	sample.w.u = src->ptr[index.y.u * src->cols + index.w.u];
	u = src_pos.x.f - (float) index.z.u;							// Normalizes to 0-1, u coordinate
	v = src_pos.y.f - (float) index.x.u;							// Normalizes to 0-1, v coordinate
	return (stt_bilerp_argb(sample, u, v));
}

// TODO: Assumption used to be scale to fit, review algorithm accordingly and add pos draw
void	ft_bilinear_scaling(const t_mat32 *src, t_mat32 *dst, t_vec2 scale, t_vec2 pos)
{
	uint32_t		x;
	uint32_t		y;
	t_vec2			norm_pos;
	const t_vec2	delta = {.x.f = 1.0 / (src->cols * scale.x.f), .y.f = 1.0 / (src->rows * scale.y.f)};

	y = 0;
	norm_pos.x.f = 0.0f;
	norm_pos.y.f = 0.0f;
	while (y < dst->rows)
	{
		x = 0;
		norm_pos.x.f = 0;
		while (x < dst->cols)
		{
			dst->ptr[y * dst->cols + x] = stt_bilerp(src, norm_pos);
			norm_pos.x.f += delta.x.f;
			x++;
		}
		norm_pos.y.f += delta.y.f;
		y++;
	}
}
