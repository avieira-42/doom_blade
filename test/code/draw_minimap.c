#include <unistd.h>
#include <math.h>
#include "render.h"
#include "ray_cast.h"

void    grid_draw(t_game *game)
{
	int	pos;

	pos = 0;
	while (pos <= 400)
	{
		if (pos == 400)
			pos = 399;
		line_draw_bresenham((t_vecf32){pos, 0},
				(t_vecf32){pos, 799},
				game, 0x777777);
		line_draw_bresenham((t_vecf32){0, pos},
				(t_vecf32){799, pos},
				game, 0x777777);
		pos += game->map.tile_x;
	}
}

void	map_draw(t_game *game)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < game->map.width)
	{
		j = 0;
		while(j < game->map.width)
		{
			if (game->map.grid[i][j] == '1')
				quad_draw((t_vecf32){j * game->map.tile_x,
						i * game->map.tile_y},
						game, 0x555555, 1);
			j++;
		}
		i++;
	}
}

void	fov_draw(t_game *game, t_player player)
{
	int32_t		i;
	float		ray_angle;
	t_vecf32	ray_target;
	t_vecf32	ray_dir;

	ray_angle = player.cam.angle - RADIANS * 960 * 0.05;
	i = 0;
	while (i < 1920)
	{
		ray_angle += RADIANS * 0.05;
		ray_target.x = player.pos.x + sinf(ray_angle) * game->vd;
		ray_target.y = player.pos.y + cosf(ray_angle) * game->vd;
		ray_dir = vec_dir(player.pos, ray_target);
		ray_cast(game, player, ray_dir);
		i++;
	}
}
