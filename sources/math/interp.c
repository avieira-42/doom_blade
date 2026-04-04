/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 09:33:45 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/04 22:20:09 by avieira-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ARGB color interpolation using SWAR technique
uint32_t	ft_lerp_argb(uint32_t p0, uint32_t p1, uint8_t alpha)
{
	uint32_t	rb;
	uint32_t	ga;

	rb = p0 & 0x00FF00FF;									// Masks red and blue channel
	ga = (p0 >> 8) & 0x00FF00FF;							// Shifts green and alpha channel then masks
	rb += (((p1 & 0x00FF00FF) - rb) * alpha) >> 8;			// 
	ga += ((((p1 >> 8) & 0x00FF00FF) - ga) * alpha) >> 8;
	return ((rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8));
}

// ARGB color interpolation using SWAR technique
uint32_t	ft_blend_argb(uint32_t p0, uint32_t p1)
{
	uint32_t		rb;
	uint32_t		ga;
	const uint32_t	alpha = p1 >> 24;

	rb = p0 & 0x00FF00FF;									// Masks red and blue channel
	ga = (p0 >> 8) & 0x00FF00FF;							// Shifts green and alpha channel then masks
	rb += (((p1 & 0x00FF00FF) - rb) * alpha) >> 8;			// 
	ga += ((((p1 >> 8) & 0x00FF00FF) - ga) * alpha) >> 8;
	return ((rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8));
}

// ARGB color interpolation using SWAR technique
uint32_t	ft_lerp_argb2(uint32_t p0, uint32_t p1, uint8_t alpha)
{
	const uint32_t	rb_p0 = p0 & 0x00FF00FF;
	const uint32_t	ga_p0 = (p0 >> 8) & 0x00FF00FF;
	const uint32_t	rb_p1 = p1 & 0x00FF00FF;
	const uint32_t	ga_p1 = (p1 >> 8) & 0x00FF00FF;
	uint32_t		rb;
	uint32_t		ga;

	rb = rb_p0 + (((rb_p1 - rb_p0) * alpha) >> 8);
	ga = ga_p0 + (((ga_p1 - ga_p0) * alpha) >> 8);
	return ((rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8));
}

// ARGB color interpolation using SWAR technique
uint32_t	ft_lerp_argb3(uint32_t p0, uint32_t p1, uint8_t alpha)
{
	const uint64_t	p0_swar = ((p0 >> 8) + ((uint64_t) p0 << 32))
		& 0x00FF00FF00FF00FF;	// R B A G
	const uint64_t	p1_swar = ((p1 >> 8) + ((uint64_t) p1 << 32))
		& 0x00FF00FF00FF00FF;	// R B A G
	uint64_t		result;

	result = p0_swar + (((p1_swar - p0_swar) * alpha) >> 8);
	result &= 0x00FF00FF00FF00FF;
	result = result >> 32 | ((result & 0xFFFFFFFF) << 8);
	return ((uint32_t)result);
}
