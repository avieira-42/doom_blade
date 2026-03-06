/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:01:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 16:32:26 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_defines.h"
#include "cub_structs.h"

void	raycast(t_cam *cam, t_mat8 *map)
{
	t_vec2	ray_dir;
	t_vec2	delta_dist;
	t_vec2	side_dist;
	t_vec2	step;
	t_vec2	map_pos;
	size_t	x;
	float	camera_x;
	int		hit = 0;
	int		side = 0;
	float	perp_wall_dist;
	int		hit_side;
	float	hit_pos_x;

	x = 0;
	while (x < RENDER_WIDTH)
	{
		camera_x = 2.0f * x / (float)RENDER_WIDTH - 1.0f;
		ray_dir.x.f = cam->dir.x.f + cam->plane.x.f * camera_x;
		ray_dir.y.f = cam->dir.y.f + cam->plane.y.f * camera_x;
		delta_dist.x.f = fabs(1.0f / (ray_dir.x.f + EPS));	// Check for zero
		delta_dist.y.f = fabs(1.0f / (ray_dir.y.f + EPS));
		map_pos.x.i = (int32_t) cam->pos.x.f;
		map_pos.y.i = (int32_t) cam->pos.y.f;
		if (ray_dir.x.f < 0)
		{
			step.x.i = -1;
			side_dist.x.f = (cam->pos.x.f - map_pos.x.i) * delta_dist.x.f;
		}
		else
		{
			step.x.i = 1;
			side_dist.x.f = (map_pos.x.i + 1.0f - cam->pos.x.f) * delta_dist.x.f;
		}
		if (ray_dir.y.f < 0)
		{
			step.y.i = -1;
			side_dist.y.f = (cam->pos.y.f - map_pos.y.i) * delta_dist.y.f;
		}
		else
		{
			step.y.i = 1;
			side_dist.y.f = (map_pos.y.i + 1.0f - cam->pos.y.f) * delta_dist.y.f;
		}
		while (hit == 0)
		{
			if (side_dist.x.f < side_dist.y.f)
			{
				side_dist.x.f += delta_dist.x.f;
				map_pos.x.i += step.x.i;
				side = 0;
			}
			else
			{
				side_dist.y.f += delta_dist.y.f;
				map_pos.y.i += step.y.i;
				side = 1;
			}
			if (map->ptr[map_pos.y.i * map->cols + map_pos.x.i] > 0)
				hit = 1;
		}
		if (side == 0)
		{
			perp_wall_dist = (map_pos.x.i - cam->pos.x.f + (1 - step.x.i) / 2) / ray_dir.x.f;
			hit_side = (step.x.i > 0) ? 3 : 1;
		}
		else
		{
			perp_wall_dist = (map_pos.y.i - cam->pos.y.f + (1 - step.y.i) / 2) / ray_dir.y.f;
			hit_side = (step.y.i > 0) ? 0 : 2;
		}
		hit_pos_x = cam->pos.x.f + perp_wall_dist * ray_dir.x.f;
		x++;
	}
}
