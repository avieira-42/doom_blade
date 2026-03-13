#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	main(int argc, char **argv)
{
	t_game		game;
	t_memory	memory;

	if (argc != 2)
		return (1);
	if (cub_init(argv[1], &game, &memory) == -1)
		return (1);
	mlx_loop(game.mlx);
}
