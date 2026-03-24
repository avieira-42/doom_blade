/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_interp.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/24 19:23:22 by adeimlin         ###   ########.fr       */
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

// row1 = stt_lerp_argb(sample.x.u, sample.y.u, u);
// row2 = stt_lerp_argb(sample.z.u, sample.w.u, u);
// result = stt_lerp_argb(row1, row2, v);
// return (result);

// static inline
// uint32_t	stt_bilerp_argb(t_vec4 sample, uint8_t u, uint8_t v)
// {

// }

static inline
uint32_t	stt_bilerp_argb(t_vec4 sample, uint8_t u, uint8_t v)
{
	uint32_t	rb;
	uint32_t	ga;
	uint32_t	row1;
	uint32_t	row2;
	uint32_t	result;

	rb = sample.x.u & 0x00FF00FF;									// Masks red and blue channel
	ga = (sample.x.u >> 8) & 0x00FF00FF;							// Shifts green and alpha channel then masks
	rb += (((sample.y.u & 0x00FF00FF) - rb) * u) >> 8;
	ga += ((((sample.y.u >> 8) & 0x00FF00FF) - ga) * u) >> 8;
	row1 = (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
	rb = sample.z.u & 0x00FF00FF;
	ga = (sample.z.u >> 8) & 0x00FF00FF;
	rb += (((sample.w.u & 0x00FF00FF) - rb) * u) >> 8;
	ga += ((((sample.w.u >> 8) & 0x00FF00FF) - ga) * u) >> 8;
	row2 = (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
	rb = row1 & 0x00FF00FF;
	ga = (row1 >> 8) & 0x00FF00FF;
	rb += (((row2 & 0x00FF00FF) - rb) * v) >> 8;
	ga += ((((row2 >> 8) & 0x00FF00FF) - ga) * v) >> 8;
	result = (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
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
	u = src_pos.x.f - (float) index.z.u;					// Normalizes to 0-1, u coordinate
	v = src_pos.y.f - (float) index.x.u;					// Normalizes to 0-1, v coordinate
	return (stt_bilerp_argb(sample, u * 255, v * 255));
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
