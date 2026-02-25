#include <stdbool.h>
#include "render.h"
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

void	ray_cast(t_game *game, t_player player, t_vecf32 r_dir, int32_t x)
{

	t_vecf32	p_pos;
	t_veci32	p_map_pos;
	t_veci32	step_ori;
	t_vecf32	step_size;
	t_vecf32	ray_len;
	t_vecf32	hit_pos;
	int32_t		color;
	int32_t		side;
	float		final_len;
	float		perp_wall_dist;
	bool		hit;

	// translate the player pos vec to float tile units p_pos.x = player.pos.x / game->map.tile_x;
	p_pos.x = player.pos.x / game->map.tile_x;
	p_pos.y = player.pos.y / game->map.tile_y;

	// translate player pos to integer tile units
	p_map_pos = (t_veci32){p_pos.x, p_pos.y};

	// set unit size for each step taken on each grid axis
	//step_size.x = 1 / u_rsqrt(1 + (r_dir.y * r_dir.y) / (r_dir.x * r_dir.x)); step_size.y = 1 / u_rsqrt(1 + (r_dir.x * r_dir.x) / (r_dir.y * r_dir.y));
	step_size.x = ft_abs(1.0f / r_dir.x);
	step_size.y = ft_abs(1.0f / r_dir.y);

	// set unit orientation for each step to take and calculate respective axis starting len
	if (r_dir.x < 0)
	{
		step_ori.x = -1;
		ray_len.x = step_size.x * ft_abs(p_pos.x - p_map_pos.x);
	}
	else
	{
		step_ori.x = 1;
		ray_len.x = step_size.x * (1 - ft_abs(p_pos.x - p_map_pos.x));
	}
	if (r_dir.y < 0)
	{
		step_ori.y = -1;
		ray_len.y = step_size.y * (ft_abs(p_pos.y - p_map_pos.y));
	}
	else
	{
		step_ori.y = 1;
		ray_len.y = step_size.y * (1 - ft_abs(p_pos.y - p_map_pos.y));
	}

	hit = false;
	final_len = 0;
	while (!hit)
	{
		if (ray_len.x < ray_len.y)
		{
			final_len = ray_len.x;
			p_map_pos.x += step_ori.x;
			ray_len.x += step_size.x;
			side = 0;
		}
		else
		{
			final_len = ray_len.y;
			p_map_pos.y += step_ori.y;
			ray_len.y += step_size.y;
			side = 1;
		}
		if (final_len >= (float)game->cam.dist)
			break;

		if (p_map_pos.x < game->map.width && p_map_pos.y < game->map.height
				&& p_map_pos.x >= 0 && p_map_pos.y >= 0)
		{
			if (game->map.grid[p_map_pos.x][p_map_pos.y] == '1')
				hit = true;
			//printf("map_x: %i\nmap_y: %i\n", p_map_pos.x, p_map_pos.y);
		}
	}
	if (hit == true)
	{
		hit_pos.x = (p_pos.x + r_dir.x * final_len) * game->map.tile_x;
		hit_pos.y = (p_pos.y + r_dir.y * final_len) * game->map.tile_y;
		color = RED;
	}
	else
	{
		hit_pos.x = (p_pos.x + r_dir.x * game->cam.dist) * game->map.tile_x;
		hit_pos.y = (p_pos.y + r_dir.y * game->cam.dist) * game->map.tile_y;
		color = GREEN;
	}
	line_draw_bresenham(game->player.pos, hit_pos, game, color);

	// 3D RENDERING

	// calculate perp_wall_dist
	if (side == 0)
		perp_wall_dist = (p_map_pos.x - p_pos.x + (1 - step_ori.x) / 2.0f) / r_dir.x;
	else
		perp_wall_dist = (p_map_pos.y - p_pos.y + (1 - step_ori.y) / 2.0f) / r_dir.y;

	//	calculate height of line to be drawn
	float	line_height = SCREEN_Y / perp_wall_dist;

	// calculate lowest and highest pixel to fill in current stripe
	t_vecf32	draw_start;
	t_vecf32	draw_end;

	draw_start.x = x;
	draw_end.x = x;
	draw_start.y = -line_height / 2 + SCREEN_Y / 2;
	draw_end.y = line_height / 2 + SCREEN_Y / 2;
	if (draw_start.x < 0)
		draw_start.x = 0;
	if (draw_end.y >= SCREEN_Y)
		draw_end.y = SCREEN_Y - 1;

	if (r_dir.x > 0)
	{
		if (r_dir.y > 0)
			color = RED;
		else
			color = BLUE;
	}
	else
	{
		if (r_dir.y > 0)
			color = WHITE;
		else
			color = GREEN;
	}
	line_draw_bresenham(draw_start, draw_end, game, color);
	printf("draw_start: %f\n", draw_start.y);
	printf("draw_end: %f\n", draw_end.y);
	//debug
}
