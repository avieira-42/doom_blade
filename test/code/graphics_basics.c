#include "types.h"
#include <unistd.h>

void	line_draw(t_vecf32 a, t_vecf32 b, t_game *game, int color)
{
	size_t			i;
	t_vecf32 const	line = vec_sub(a, b);
	int		x;
	int		y;
	size_t const	max = vec_max_coord(a);

	if (max != 0)
	{
		x = line.x / max;
		y = line.y / max;
		i = 0;
		while (i < max)
		{
			a.x += x;
			a.y += y;
			map_img_pixel_put(game, a.x , a.y, color);
			i++;
		}
	}
}
