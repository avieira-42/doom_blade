#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"
#include "cub_structs.h"

static
int	stt_cub_flood_fill(t_mat8 map, int32_t x, int32_t y)
{
	uint8_t	block;

	if (x >= map.cols || x <= -1 || y >= map.rows || y <= -1)
		return (-1);
	block = map.ptr[map.cols * x + map.rows * y];
	if (block != 0)
		return (0);
	map.ptr[map.cols * x + map.rows * y] = '1';
	if (stt_cub_flood_fill(map, x + 1, y) == -1)
		return (-1);
	if (stt_cub_flood_fill(map, x - 1, y) == -1)
		return (-1);
	if (stt_cub_flood_fill(map, x, y + 1) == -1)
		return (-1);
	if (stt_cub_flood_fill(map, x, y - 1) == -1)
		return (-1);
	return (0);
}

int	cub_is_map_enclosed(t_mat8 map, t_vec2 player_pos)
{
	return (stt_cub_flood_fill(map, player_pos.x.i, player_pos.y.i));
}
