/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 14:50:05 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 16:56:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/time.h>
#include "cub_utils.h"

t_sides	cub_center_clip(t_mat32 frame, t_vec2 draw_pos, t_vec2 new_size)
{
	t_sides	bounds;

	bounds.x = draw_pos.x.i - new_size.x.i / 2;
	bounds.y = draw_pos.y.i - new_size.y.i / 2;
	bounds.left = ft_iclamp(bounds.x, 0, frame.width);
	bounds.right = ft_iclamp(draw_pos.x.i + new_size.x.i / 2, 0, frame.width);
	bounds.top = ft_iclamp(bounds.y, 0, frame.height);
	bounds.bottom = ft_iclamp(draw_pos.y.i + new_size.y.i / 2, 0, frame.height);
	return (bounds);
}

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

long	get_time(void)
{
	static long		prev_time = 0;
	long			dt;
	struct timeval	now;

	dt = -prev_time;
	gettimeofday(&now, NULL);
	prev_time = now.tv_sec * 1000000 + now.tv_usec;
	dt += prev_time;
	return (dt);
}

long	get_time_v2(void)
{
	static long		prev_time[8];	// Stores the result of the last N measurements
	static size_t	index = 0;
	long			dt;
	struct timeval	now;

	gettimeofday(&now, NULL);
	dt = -prev_time[index];
	index = (index + 1) % 8;
	prev_time[index] = now.tv_sec * 1000000 + now.tv_usec;
	dt += prev_time[index];
	return (dt);
}
