#include "types.h"

void	img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_X || y < 0 || y >= SCREEN_Y)
		return ;
	dst = game->img.addr + (y * game->map_2d.l_len + x * (game->map_2d.bpp / 8));
	*(unsigned int *)dst = color;
}
