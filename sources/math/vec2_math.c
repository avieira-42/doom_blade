/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec2_math.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:29:46 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/08 15:22:36 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx_base.h"

t_vec2	vec2_sum(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){.x.f = a.x.f + b.x.f, .y.f = a.y.f + b.y.f});
}

t_vec2	vec2_mult(t_vec2 a, float scalar)
{
	return ((t_vec2){.x.f = a.x.f * scalar, .y.f = a.y.f * scalar});
}

t_vec2	vec2_sub(t_vec2 a, t_vec2 b)
{
	return ((t_vec2){.x.f = b.x.f - a.x.f, .y.f = b.y.f - a.y.f});
}

float	vec2_max(t_vec2 vec)
{
	vec.x.u &= 0x7FFFFFFF;
	vec.y.u &= 0x7FFFFFFF;
	if (vec.x.f > vec.y.f)
		return (vec.x.f);
	else
		return (vec.y.f);
}
