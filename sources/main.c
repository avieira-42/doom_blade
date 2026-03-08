#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cub_structs.h"

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (1);
	if (parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
}
