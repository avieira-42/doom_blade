/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rng_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:31:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 15:31:13 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include "cmlx_base.h"
#include "cub_structs.h"
#include "cub_utils.h"

t_vec2	random_valid_pos(t_map *map)
{
	uint16_t	x;
	uint16_t	y;
	t_vec2		pos;

	while (true)
	{
		pos.x.f = 1.5f + floorf(ft_randf() * (map->width - 2));
		pos.y.f = 1.5f + floorf(ft_randf() * (map->height - 2));
		x = (uint16_t) floorf(pos.x.f);
		y = (uint16_t) floorf(pos.y.f);
		if (map->tex_index[y * map->width + x] <= 127)
			break ;
	}
	return (pos);
}
