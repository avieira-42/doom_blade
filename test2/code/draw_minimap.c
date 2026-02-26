#include <unistd.h>
#include "render.h"
#include "ray_cast.h"

void    grid_draw(t_game *game, t_vecf32 map_max,
		t_vecf32 map_tile, t_veci32 map_size)
{
	int	i;
	float	pos;

	i = 0;
	pos = 0;
	while (i <= map_size.y)
	{
		line_draw_bresenham((t_vecf32){0, pos},
				(t_vecf32){map_max.x, pos},
				game, 0x777777);
		pos += map_tile.y;
		i++;
	}
	i = 0;
	pos = 0; while (i <= map_size.x)
	{
		line_draw_bresenham((t_vecf32){pos, 0},
				(t_vecf32){pos, map_max.y},
				game, 0x777777);
		pos += map_tile.x;
		i++;
	}
}

void	objects_draw(t_game *game, t_veci32 map_size, t_vecf32 map_tile)
{
	int32_t	y;
	int32_t	x;

	y = 0;
	while (y < map_size.y)
	{
		x = 0;
		while(x < map_size.x)
		{
			if (game->map.grid[x][y] == '1')
				quad_draw((t_vecf32){x * map_tile.x,
						y * map_tile.y},
						game, 0x555555, map_tile);
			x++;
		}
		y++;
	}
}

void	fov_draw(t_game *game)
{
	int32_t		x;
	float		camera_x;
	t_vecf32	ray_dir;

	x = 0;
	while(x < SCREEN_X)
	{
		camera_x = 2 * x / (float)SCREEN_X - 1;
		ray_dir.x = game->player.dir.x + game->cam.dir.x * camera_x;
		ray_dir.y = game->player.dir.y + game->cam.dir.y * camera_x;
		ray_cast(game, game->player, ray_dir, x);
		x++;
	}
}

void	player_draw(t_game *game, t_vecf32 p_pos, t_vecf32 map_tile)
{
	quad_draw(p_pos, game, 0xFF00FF, map_tile);
	img_pixel_put(game, game->player.pos.x, game->player.pos.y, 0xFFFFFF);
}

void	minimap_draw(t_game *game)
{
	t_vecf32 const	map_max = (t_vecf32){game->map.max_x, game->map.max_y};
	t_vecf32 const	map_tile = (t_vecf32){game->map.tile_x, game->map.tile_y};
	t_veci32 const	map_size = (t_veci32){game->map.width, game->map.height};
	t_vecf32		p_pos;

	fov_draw(game);
	//quad_draw((t_vecf32){0, 0}, game, BLACK, map_max);
	objects_draw(game, map_size, map_tile);
	grid_draw(game, map_max, map_tile, map_size);
	p_pos.x = game->player.pos.x - game->map.tile_x * 0.5;
	p_pos.y = game->player.pos.y - game->map.tile_y * 0.5;
	// to remove after collisions
	if (game->player.pos.x <= game->map.max_x
			&& game->player.pos.y <= game->map.max_y)
		player_draw(game, p_pos, map_tile);
	// to remove after collisions
}
