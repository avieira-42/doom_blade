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

// static
// int	stt_validate_map(t_mat8 *map)
// {
// 	size_t	i;
// 	uint8_t	*ptr1;
// 	uint8_t	*ptr2;

// 	i = 0;
// 	ptr1 = map->ptr;
// 	ptr2 = map->ptr + map->rows * map->cols;	// Possible off by one
// 	while (ptr1[i] == '1' && ptr2[i] && i < map->cols)
// 		i++;
// 	if (i != map->cols)
// 		return (-1);
// 	i = 0;
// 	ptr2 = map->ptr + map->cols;
// 	while (ptr1[i] == '1' && ptr2[i] && i < map->rows)
// 		i += map->cols;
// 	if (i != map->rows)
// 		return (-1);
// 	return (0);
// }