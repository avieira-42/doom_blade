/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 15:01:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 15:21:35 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"

void	ray_cast_init(t_player *player, t_ray *ray)
{
	ray->pos = (t_vec2){player->pos.x, player->pos.y};
	ray->map_cell = (t_vec2){ray->pos.x, ray->pos.y};
	ray->step_size.x.f = ft_abs(1.0f / ray->ray_dir.x.f);
	ray->step_size.y.f = ft_abs(1.0f / ray->ray_dir.y.f);
	ray->hit = false;
	ray->distance = 0;
	if (ray->ray_dir.x.f < 0)
	{
		ray->step.x.i = -1;
		ray->side_dist.x.f = ray->step_size.x.f * ft_abs(ray->pos.x.f - ray->map_cell.x.i);
	}
	else
	{
		ray->step.x.i = 1;
		ray->side_dist.x.f = ray->step_size.x.f * (1 - ft_abs(ray->pos.x.f - ray->map_cell.x.i));
	}
	if (ray->ray_dir.y.f < 0)
	{
		ray->step.y.i = -1;
		ray->side_dist.y.f = ray->step_size.y.f * (ft_abs(ray->pos.y.f - ray->map_cell.y.i));
	}
	else
	{
		ray->step.y.i = 1;
		ray->side_dist.y.f = ray->step_size.y.f * (1 - ft_abs(ray->pos.y.f - ray->map_cell.y.i));
	}
}

void	ray_cast(t_game *game, t_ray *ray)
{
	ray_cast_init(&game->player, ray);
	while (!ray->hit)
	{
		if (ray->side_dist.x.f < ray->side_dist.y.f)
		{
			ray->distance = ray->side_dist.x.f;
			ray->map_cell.x.i += ray->step.x.i;
			ray->side_dist.x.f += ray->step_size.x.f;
			ray->hit_side = 0;
		}
		else
		{
			ray->distance = ray->side_dist.y.f;
			ray->map_cell.y.i += ray->step.y.i;
			ray->side_dist.y.f += ray->step_size.y.f;
			ray->hit_side = 1;
		}
		if (ray->distance >= (float)game->vd)
			break;
		if (ray->map_cell.x.i < game->map.cols && ray->map_cell.y.i < game->map.rows && ray->map_cell.x.i >= 0 && ray->map_cell.y.i >= 0)
			if (game->map.ptr[ray->p_map_pos.y.i][ray->p_map_pos.x.i] == '1')	// To do: index properly
				ray->hit = true;
	}
}

void	space_render(t_game *game)
{
	size_t		x;
	float		camera_x;
	t_ray		ray;
	t_vec2	p_pos;
	t_vec2 const	map_max = (t_vec2){game->map.max_x, game->map.max_y};
	t_vec2 const	map_tile = (t_vec2){game->map.tile_x, game->map.tile_y};
	t_vec2 const	map_size = (t_vec2){game->map.width, game->map.height};

	x = 0;
	while(x < RENDER_WIDTH)
	{
		camera_x = 2 * x / RENDER_WIDTH - 1;
		ray.ray_dir.x = game->player.dir.x + game->cam.dir.x * camera_x;
		ray.ray_dir.y = game->player.dir.y + game->cam.dir.y * camera_x;
		ray_cast(game, &ray);
		//////////////////////////////
		

		p_pos.x = game->player.pos.x * game->map.tile_x;
		p_pos.y = game->player.pos.y * game->map.tile_y;
		if (ray.hit == true)
		{
			ray.hit_pos.x = (ray.pos.x + ray.ray_dir.x * ray.distance) * game->map.tile_x;
			ray.hit_pos.y = (ray.pos.y + ray.ray_dir.y * ray.distance) * game->map.tile_y;
			ray.color = RED;
		}
		else
		{
			ray.hit_pos.x = (ray.pos.x + ray.ray_dir.x * game->cam.dist) * game->map.tile_x;
			ray.hit_pos.y = (ray.pos.y + ray.ray_dir.y * game->cam.dist) * game->map.tile_y;
			ray.color = GREEN;
		}
		// line_draw_bresenham(p_pos, ray.hit_pos, game, ray.color);
		if (!ray.hit)
			return ;
		// column_render(game, ray, player, x);
		////////////////////////////// 
		x++;
	}
}
