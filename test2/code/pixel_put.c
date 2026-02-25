#include <unistd.h>
#include "types.h"

void	img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= SCREEN_X || y < 0 || y >= SCREEN_Y)
		return ;
	dst = game->frame.addr + (y * game->frame.l_len + x * (game->frame.bpp / 8));
	*(unsigned int *)dst = color;
}

void	window_clear(t_game *game, int color)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < SCREEN_Y)
	{
		j = 0;
		while (j < SCREEN_X)
		{
			img_pixel_put(game, j, i, color);
			j++;
		}
		i++;
	}
}
