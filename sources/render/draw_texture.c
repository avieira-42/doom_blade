#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_texture(t_mat32 frame, t_mat32 image, size_t xc, size_t yc)
{
	size_t		x;
	size_t		y;
	uint32_t	color;

	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x * image.stride + y];
			if (color != 2228223 && color != 1441791)	// REVIEW: colors are hex coded normally
				pixel_put(frame, x + xc, y + yc, color);
			x++;
		}
		y++;
	}
}
