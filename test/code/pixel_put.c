#include <unistd.h>
#include "types.h"

void	map_img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= 400 || y < 0 || y >= 400)
		return ;
	dst = game->map_2d.addr + (y * game->map_2d.l_len + x * (game->map_2d.bpp / 8));
	*(unsigned int *)dst = color;
}

void	screen_img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= 1920 || y < 0 || y >= 1024)
		return ;
	dst = game->map_3d.addr + (y * game->map_3d.l_len + x * (game->map_3d.bpp / 8));
	*(unsigned int *)dst = color;
}

void	screen_img_column_put(t_game *game, int x, float len, int color)
{
	int			y;
	float const	scale = 1024 / len;
	float const	limit = scale / 2;
	float const	draw_area = 1024 - limit * 2;
	//printf("scale: %f\n", scale);
	//printf("len: %f\n", len);

	y = 0;
	while (y <= 1920)
	{
		if (y >= limit && y <= limit + draw_area)
			screen_img_pixel_put(game, x, y, color);
		y++;
	}
}

void	window_clear(t_game *game, int color)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < 1920)
	{
		j = 0;
		while (j < 1920)
		{
			map_img_pixel_put(game, j, i, color);
			screen_img_pixel_put(game, j, i, color);
			j++;
		}
		i++;
	}
}
