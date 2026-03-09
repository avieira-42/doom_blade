#include <stdlib.h>
#include "cmlx.h"
#include "cub_defines.h"
#include "cub_structs.h"

// Cleanup process
static
int	stt_cleanup(t_game *game)
{
	// size_t	i;

	// free(game->map.ptr);
	// i = 0;
	// while (i < NUM_BLOCKS)
	// {
	// 	free(game->blocks[i].north.ptr);		// Careful with double free of repeated ptrs
	// 	free(game->blocks[i].south.ptr);
	// 	free(game->blocks[i].east.ptr);
	// 	free(game->blocks[i].west.ptr);
	// }
}

int	cub_cleanup(t_game *game)
{
	exit(1);
}