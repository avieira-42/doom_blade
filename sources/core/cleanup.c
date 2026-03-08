#include <stdlib.h>
#include "cmlx.h"
#include "cub_defines.h"
#include "cub_structs.h"

// Cleanup process
static
int	stt_cleanup(t_game *game)
{
	size_t	i;

	free(map->ptr);
	i = 0;
	while (i < NUM_BLOCKS)
	{
		free(map->blocks[i].north.ptr);		// Careful with double free of repeated ptrs
		free(map->blocks[i].south.ptr);
		free(map->blocks[i].east.ptr);
		free(map->blocks[i].west.ptr);
	}
}

int	cub_cleanup(t_xvar *mlx, t_map *map)
{
	
}