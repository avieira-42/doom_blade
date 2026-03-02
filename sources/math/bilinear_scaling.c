/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bilinear_scaling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/01 18:09:31 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "tmp_math.h"

uint32_t	ft_lerp_argb(uint32_t p0, uint32_t p1, uint8_t alpha)
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
uint32_t	stt_interp(uint32_t *src1, uint32_t *src2, float u, float v)
{
	uint32_t	row1;
	uint32_t	row2;
	uint32_t	result;

	row1 = ft_lerp_argb(src1[0], src1[1], u * 255);
	row2 = ft_lerp_argb(src2[0], src2[1], u * 255);
	result = ft_lerp_argb(row1, row2, v * 255);
	return (result);
}

// Assumption is scale to fit
void	ft_bilinear_scaling(t_image *src, t_image *dst)
{
	
}
