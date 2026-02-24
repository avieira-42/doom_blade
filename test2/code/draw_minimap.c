#include <unistd.h>
#include "render.h"

void    grid_draw(t_game *game)
{
	int32_t	pos;
	int32_t	const	max_x = game->map.width * game->map.tile_x;
	int32_t const	max_y = game->map.height * game->map.tile_y;

	pos = 0;
	while (pos <= max_y)
	{
		line_draw_bresenham((t_vecf32){0, pos},
				(t_vecf32){max_x, pos},
				game, 0x777777);
		pos += game->map.tile_y;
	}
	pos = 0; while (pos <= max_x)
	{
		line_draw_bresenham((t_vecf32){pos, 0},
				(t_vecf32){pos, max_y},
				game, 0x777777);
		pos += game->map.tile_x;
	}
}

void	objects_draw(t_game *game)
{
	int32_t	y;
	int32_t	x;

	y = 0;
	while (y < game->map.height)
	{
		x = 0;
		while(x < game->map.width)
		{
			if (game->map.grid[x][y] == '1')
				quad_draw((t_vecf32){x * game->map.tile_x,
						y * game->map.tile_y},
						game, 0x555555, 1);
			x++;
		}
		y++;
	}
}

void	fov_draw(t_game *game, t_player player, t_cam cam)
{
	(void)game;
	(void)player;
	(void)cam;
}

void	player_draw(t_game *game)
{
	int32_t	const	max_x = game->map.width * game->map.tile_x;
	int32_t const	max_y = game->map.height * game->map.tile_y;

	if (game->player.pos.x <= max_x && game->player.pos.y <= max_y)
		quad_draw(game->player.pos, game, 0xFF00FF, 1);
}

void	minimap_draw(t_game *game)
{
	objects_draw(game);
	grid_draw(game);
	player_draw(game);
}
