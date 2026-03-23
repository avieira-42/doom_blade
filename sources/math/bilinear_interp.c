/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_interp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/23 17:09:05 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

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

uint32_t	ft_bilerp_argb(const t_mat32 *src, t_vec2 norm_pos)
{
	t_vec2		src_pos;
	float		u;
	float		v;
	t_vec4		index;
	t_vec4		sample;

	src_pos.x.f = norm_pos.x.f * (src->width - 1);
	src_pos.y.f = norm_pos.y.f * (src->height - 1);
	index.x.u = src_pos.y.f;
	index.y.u = src_pos.y.f + (index.x.u < (src->height - 1));
	index.z.u = src_pos.x.f;
	index.w.u = src_pos.x.f + (index.z.u < (src->width - 1));
	sample.x.u = src->ptr[index.x.u * src->stride + index.z.u];
	sample.y.u = src->ptr[index.x.u * src->stride + index.w.u];
	sample.z.u = src->ptr[index.y.u * src->stride + index.z.u];
	sample.w.u = src->ptr[index.y.u * src->stride + index.w.u];
	u = src_pos.x.f - (float) index.z.u;							// Normalizes to 0-1, u coordinate
	v = src_pos.y.f - (float) index.x.u;							// Normalizes to 0-1, v coordinate
	return (stt_bilerp_argb(sample, u, v));
}

uint32_t	ft_bilerp_argb_t(const t_mat32 *src, t_vec2 norm_pos)
{
	t_vec2		src_pos;
	float		u;
	float		v;
	t_vec4		index;
	t_vec4		sample;

	src_pos.x.f = norm_pos.x.f * (src->width - 1);
	src_pos.y.f = norm_pos.y.f * (src->height - 1);
	index.x.u = src_pos.y.f;
	index.y.u = src_pos.y.f + (index.x.u < (src->height - 1));
	index.z.u = src_pos.x.f;
	index.w.u = src_pos.x.f + (index.z.u < (src->width - 1));
	sample.x.u = src->ptr[index.x.u + src->stride * index.z.u];
	sample.y.u = src->ptr[index.x.u + src->stride * index.w.u];
	sample.z.u = src->ptr[index.y.u + src->stride * index.z.u];
	sample.w.u = src->ptr[index.y.u + src->stride * index.w.u];
	u = src_pos.x.f - (float) index.z.u;							// Normalizes to 0-1, u coordinate
	v = src_pos.y.f - (float) index.x.u;							// Normalizes to 0-1, v coordinate
	return (stt_bilerp_argb(sample, u, v));
}
