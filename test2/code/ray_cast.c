#include <stdbool.h>
#include "types.h"
#include "libft_math.h"


	/*For the dda ray_cast algorithm to work, all the coordinates
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

void	ray_cast_init(t_cam cam, t_map map, t_ray *ray)
{
	ray->p_pos = (t_vecf32){cam.pos.x / map.tile_x, cam.pos.y / map.tile_y};
	ray->p_map_pos = (t_veci32){ray->p_pos.x, ray->p_pos.y};
	ray->step_size.x = ft_abs(1.0f / ray->dir.x);
	ray->step_size.y = ft_abs(1.0f / ray->dir.y);
	ray->hit = false;
	ray->final_len = 0;
	if (ray->dir.x < 0)
	{
		ray->step_ori.x = -1;
		ray->ray_len.x = ray->step_size.x * ft_abs(ray->p_pos.x - ray->p_map_pos.x);
	}
	else
	{
		ray->step_ori.x = 1;
		ray->ray_len.x = ray->step_size.x * (1 - ft_abs(ray->p_pos.x - ray->p_map_pos.x));
	}
	if (ray->dir.y < 0)
	{
		ray->step_ori.y = -1;
		ray->ray_len.y = ray->step_size.y * (ft_abs(ray->p_pos.y - ray->p_map_pos.y));
	}
	else
	{
		ray->step_ori.y = 1;
		ray->ray_len.y = ray->step_size.y * (1 - ft_abs(ray->p_pos.y - ray->p_map_pos.y));
	}
}

void	ray_cast(t_game *game, t_ray *ray)
{
	ray_cast_init(game->cam, game->map, ray);
	while (!ray->hit)
	{
		if (ray->ray_len.x < ray->ray_len.y)
		{
			ray->final_len = ray->ray_len.x;
			ray->p_map_pos.x += ray->step_ori.x;
			ray->ray_len.x += ray->step_size.x;
			ray->side = 0;
		}
		else
		{
			ray->final_len = ray->ray_len.y;
			ray->p_map_pos.y += ray->step_ori.y;
			ray->ray_len.y += ray->step_size.y;
			ray->side = 1;
		}
		if (ray->final_len >= (float)game->vd)
			break;
		if (ray->p_map_pos.x < game->map.width
				&& ray->p_map_pos.y < game->map.height
				&& ray->p_map_pos.x >= 0 && ray->p_map_pos.y >= 0)
			if (game->map.grid[ray->p_map_pos.y][ray->p_map_pos.x] == '1')
				ray->hit = true;
	}
}
