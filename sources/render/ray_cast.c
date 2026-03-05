/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 17:52:55 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 18:04:46 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

/* For the dda ray_cast algorithm to work, all the coordinates
	must be translated to tile map unit coordinates, with
	the small caveat that theres the necessity of storing
	two different data types of the player position, one
	as integer number, so the collision is found, the other
	as float number, so the length of the ray (to be translated
	at the end) is preserved, due to possibility of multiple
	points of collision with the same Wall.
For such, the algorithm requires the creatio of several
	variables:
		1.	The direction of the ray that is being casted
				that is already normalized.
		2.	The player position in tile units (float) needed
				to calculate variable 4.
		3.	The player position in tile units (integer)
				that we will increase with each step taken
		4.	The length of the ray respective to each
				axis step taken in the grid
		5.	The axis step orientation, according to the
				respective axis of the ray direction
		6.	A boolean to check for the collision of the
				ray and get out of the casting loop
		7.	The translation of the hit position back to
				world units (float)
		8.	The ray final len to set with whichever axis
				len corresponds to the next collision
				point to break out of the loop in case
				of maximum view distance reached
	while ray < view_distance
		if x_ray <= y_ray
			ray = x_ray
   			increase x
   			calculate x_ray
   		else
			ray = y_ray
   			increase y
   			calculate y_ray
   		check ray collision */

void	ray_cast_init(t_player *player, t_ray *ray)
{
	ray->p_pos = (t_vec2){player->pos.x, player->pos.y};
	ray->p_map_pos = (t_vec2){ray->p_pos.x, ray->p_pos.y};
	ray->step_size.x.f = ft_abs(1.0f / ray->dir.x.f);
	ray->step_size.y.f = ft_abs(1.0f / ray->dir.y.f);
	ray->hit = false;
	ray->final_len = 0;
	if (ray->dir.x.f < 0)
	{
		ray->step_ori.x.i = -1;
		ray->ray_len.x.f = ray->step_size.x.f * ft_abs(ray->p_pos.x.f - ray->p_map_pos.x.i);
	}
	else
	{
		ray->step_ori.x.i = 1;
		ray->ray_len.x.f = ray->step_size.x.f * (1 - ft_abs(ray->p_pos.x.f - ray->p_map_pos.x.i));
	}
	if (ray->dir.y.f < 0)
	{
		ray->step_ori.y.i = -1;
		ray->ray_len.y.f = ray->step_size.y.f * (ft_abs(ray->p_pos.y.f - ray->p_map_pos.y.i));
	}
	else
	{
		ray->step_ori.y.i = 1;
		ray->ray_len.y.f = ray->step_size.y.f * (1 - ft_abs(ray->p_pos.y.f - ray->p_map_pos.y.i));
	}
}

void	ray_cast(t_game *game, t_ray *ray)
{
	ray_cast_init(&game->player, ray);
	while (!ray->hit)
	{
		if (ray->ray_len.x.f < ray->ray_len.y.f)
		{
			ray->final_len = ray->ray_len.x.f;
			ray->p_map_pos.x.i += ray->step_ori.x.i;
			ray->ray_len.x.f += ray->step_size.x.f;
			ray->side = 0;
		}
		else
		{
			ray->final_len = ray->ray_len.y.f;
			ray->p_map_pos.y.i += ray->step_ori.y.i;
			ray->ray_len.y.f += ray->step_size.y.f;
			ray->side = 1;
		}
		if (ray->final_len >= (float)game->vd)
			break;
		if (ray->p_map_pos.x.i < game->map.cols
				&& ray->p_map_pos.y.i < game->map.rows
				&& ray->p_map_pos.x.i >= 0 && ray->p_map_pos.y.i >= 0)
			if (game->map.ptr[ray->p_map_pos.y.i][ray->p_map_pos.x.i] == '1')	// To do: index properly
				ray->hit = true;
	}
}
