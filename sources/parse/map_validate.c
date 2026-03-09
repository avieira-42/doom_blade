#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_types.h"

int	cub_is_map_enclosed(t_mat8 map)
{
	size_t	x;
	size_t	y;
	uint8_t	block;

	y = 0;
	while (y < map.rows)
	{
		x = 0;
		while(x < map.cols)
		{
			block = map.ptr[x + map.cols * y];
			if ((x == 0 || x == map.cols - 1 || y == 0 || y == map.rows - 1)
					&& block != '1')
				return (-1);
			x++;
		}
		y++;
	}
	return (0);
}
