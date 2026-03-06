/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 16:35:01 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 16:57:46 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "math.h"

static
float	stt_block_dir(t_cam *cam, t_vec2 ray_dir, t_vec2 map_pos, int side)
{
	t_vec2	step;
	float	perp_dist;
	int		hit_index;
	float	hit_pos_x;

	if (ray_dir.x.f < 0)
		step.x.i = -1;
	else
		step.x.i = 1;
	if (ray_dir.y.f < 0)
		step.y.i = -1;
	else
		step.y.i = 1;
	if (side == 0)
	{
		perp_dist = (map_pos.x.i - cam->pos.x.f + (1 - step.x.i) / 2) / ray_dir.x.f;
		hit_index = 1 + ((step.x.i > 0) << 1);
	}
	else
	{
		perp_dist = (map_pos.y.i - cam->pos.y.f + (1 - step.y.i) / 2) / ray_dir.y.f;
		hit_index = (step.y.i <= 0) << 1;
	}
	hit_pos_x = cam->pos.x.f + perp_dist * ray_dir.x.f;
	return (hit_pos_x + (float)hit_index);
}

static
int	stt_dda(t_cam *cam, t_vec2 ray_dir, t_vec2 *map_pos, t_mat8 *map)
{
	t_vec2	delta_dist;
	t_vec2	side_dist;
	t_vec2	step;
	int		hit;
	int		side;

	delta_dist.x.f = fabs(1.0f / (ray_dir.x.f + EPS));
	delta_dist.y.f = fabs(1.0f / (ray_dir.y.f + EPS));
	step.x.i = (ray_dir.x.f < 0) ? -1 : 1;
	step.y.i = (ray_dir.y.f < 0) ? -1 : 1;
	if (ray_dir.x.f < 0)
		side_dist.x.f = (cam->pos.x.f - map_pos->x.i) * delta_dist.x.f;
	else
		side_dist.x.f = (map_pos->x.i + 1.0f - cam->pos.x.f) * delta_dist.x.f;
	if (ray_dir.x.f < 0)
		side_dist.y.f = (cam->pos.y.f - map_pos->y.i) * delta_dist.y.f;
	else
		side_dist.y.f = (map_pos->y.i + 1.0f - cam->pos.y.f) * delta_dist.y.f;
	hit = 0;
	while (hit == 0)
	{
		if (side_dist.x.f < side_dist.y.f)
		{
			side_dist.x.f += delta_dist.x.f;
			map_pos->x.i += step.x.i;
			side = 0;
		}
		else
		{
			side_dist.y.f += delta_dist.y.f;
			map_pos->y.i += step.y.i;
			side = 1;
		}
		if (map->ptr[map_pos->y.i * map->cols + map_pos->x.i] > 0)
			hit = 1;	// Return hit type
	}
	return (side);
}

void	raycast(t_cam *cam, t_mat8 *map)
{
	size_t	x;
	t_vec2	ray_dir;
	t_vec2	map_pos;
	int		side;
	float	camera_x;

	x = 0;
	while (x < RENDER_WIDTH)
	{
		camera_x = 2.0f * x / (float)RENDER_WIDTH - 1.0f;
		ray_dir.x.f = cam->dir.x.f + cam->plane.x.f * camera_x;
		ray_dir.y.f = cam->dir.y.f + cam->plane.y.f * camera_x;
		map_pos.x.i = (int32_t)cam->pos.x.f;
		map_pos.y.i = (int32_t)cam->pos.y.f;
		side = stt_dda(cam, ray_dir, &map_pos, map);
		stt_block_dir(cam, ray_dir, map_pos, side);
		x++;
	}
}
