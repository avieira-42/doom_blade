/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   float_sqrt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/13 10:28:32 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 13:04:17 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include "cub_structs.h"

// This is faster than receiving a t_32 (thank god)
float	ft_qinvsqrt(float number)
{
	t_32	result;

	result.f = number;
	result.u = 0x5f3759df - (result.u >> 1);
	result.f = result.f * (1.5f - (number * 0.5f * result.f * result.f));
	return (result.f);
}

float	ft_qsqrt(float number)
{
	t_32	result;

	result.f = number;
	result.u = (result.u >> 1) + 0x1FBD3EE0;
	result.f = 0.5f * (result.f + number / result.f);
	return (result.f);
}
