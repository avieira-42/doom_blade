#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"
#include "cub_structs.h"

int	loop(t_mat8 *map, t_entity *player)
{
	size_t	i;
	size_t	j;
	uint8_t	block;

	i = 0;
	while (i < map->cols)
	{
		j = 0;
		while (j < map->rows)
		{
			block = map->ptr[i * map->cols + j * map->rows];
			j++;
		}
		i++;
	}
}
