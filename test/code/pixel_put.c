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
