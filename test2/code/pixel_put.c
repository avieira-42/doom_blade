#include <unistd.h>
#include "types.h"

void	img_pixel_put(t_game *game, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= 1920 || y < 0 || y >= 1024)
		return ;
	dst = game->frame.addr + (y * game->frame.l_len + x * (game->frame.bpp / 8));
	*(unsigned int *)dst = color;
}

void	window_clear(t_game *game, int color)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < 1024)
	{
		j = 0;
		while (j < 1920)
		{
			img_pixel_put(game, j, i, color);
			j++;
		}
		i++;
	}
}
