#include <math.h>
#include <stdbool.h>
#include "render.h"
#include "types.h"

int32_t	ray_cast_size(t_game *game)
{
	return (2 * tanf(game->fov * 0.5) * 2 * game->vd);
}

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
void	ray_cast(t_game *game, t_player player, t_vecf32 r_dir)
{

	t_vecf32	p_pos;
	t_veci32	p_map_pos;
	t_veci32	step_ori;
	t_vecf32	step_size;
	t_vecf32	ray_len;
	t_vecf32	hit_pos;
	float		final_len;
	//int			color;
	bool		hit;

	// translate the player pos vec to float tile units
	p_pos.x = player.center.x / game->map.tile_x;
	p_pos.y = player.center.y / game->map.tile_y;
	//printf("p_pos.x: %f\np_pos.y: %f\n", p_pos.x, p_pos.y);

	// translate player pos to integer tile units
	p_map_pos = (t_veci32){p_pos.x, p_pos.y};
	//printf("p_map_pos.x: %i\np_map_pos.y: %i\n", p_map_pos.x, p_map_pos.y);

	// set unit size for each step taken on each grid axis
	step_size.x = 1 / u_rsqrt(1 + (r_dir.y * r_dir.y) / (r_dir.x * r_dir.x));
	step_size.y = 1 / u_rsqrt(1 + (r_dir.x * r_dir.x) / (r_dir.y * r_dir.y));
	//printf("step_size.x: %f\nstep_size.y: %f\n", step_size.x, step_size.y);

	// set unit orientation for each step to take and calculate respective axis starting len
	if (r_dir.x < 0)
	{
		step_ori.x = -1;
		ray_len.x = step_size.x * ft_abs(p_pos.x - p_map_pos.x) - 0.05;
		//printf("step_ori.x: %i\n ray_len.x: %f\n", step_ori.x, ray_len.x);
	}
	else
	{
		step_ori.x = 1;
		ray_len.x = step_size.x * (1 - ft_abs(p_pos.x - p_map_pos.x));
		//printf("step_ori.x: %i\n ray_len.x: %f\n", step_ori.x, ray_len.x);
	}
	if (r_dir.y < 0)
	{
		step_ori.y = -1;
		ray_len.y = step_size.y * (ft_abs(p_pos.y - p_map_pos.y)) - 0.06;
		//printf("step_ori.y: %i\n ray_len.y: %f\n", step_ori.y, ray_len.y);
	}
	else
	{
		step_ori.y = 1;
		ray_len.y = step_size.y * (1 - ft_abs(p_pos.y - p_map_pos.y));
		//printf("step_ori.y: %i\n ray_len.y: %f\n", step_ori.y, ray_len.y);
	}

	hit = false;
	final_len = 0;
	while (!hit && final_len < game->player.cam.dist / game->map.tile_x)
	{
		if (ray_len.x < ray_len.y)
		{
			final_len = ray_len.x;
			p_map_pos.x += step_ori.x;
			ray_len.x += step_size.x;
			//hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
			//hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
			//quad_draw(hit_pos, game, 0x0000FF, 0.3);
		}
		else
		{
			final_len = ray_len.y;
			p_map_pos.y += step_ori.y;
			ray_len.y += step_size.y;
			//hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
			//hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
			//quad_draw(hit_pos, game, 0xFF00FF, 0.3);
		}

		if (p_map_pos.x < game->map.width && p_map_pos.y < game->map.height
				&& p_map_pos.x >= 0 && p_map_pos.y >= 0)
		{
			if (game->map.grid[p_map_pos.y][p_map_pos.x] == '1')
				hit = true;
		}
	}
	if (hit == true)
	{
		hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
		//printf("hit_pos.x: %f\n", hit_pos.x);
		hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
		//quad_draw(hit_pos, game, 0xFF0000, 1);
		//screen_img_column_put(game, (p_pos.y + r_dir.y * final_len) * 128, final_len, color);
	}
	line_draw_bresenham(game->player.center, hit_pos, game, 0x00FF00);
}
