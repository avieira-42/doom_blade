#include <SDL3/SDL_main.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "game_types.h"
#include "game_defines.h"
#include "game_prototypes.h"

static
void	stt_loop(t_game *game)
{
	SDL_Surface	*window_surface = SDL_GetWindowSurface(game->window);	// Why is this inside the loop?

	if (game->state.paused == false)
	{
		game_update(game);
		SDL_BlitSurface(game->frame.img, NULL, window_surface, NULL);
		SDL_UpdateWindowSurface(game->window);
	}
}

int	main(int argc, char **argv)
{
	static t_game	game;
	static t_memory	memory;

	if (argc != 2)
		return (write(2, "Error\nInvalid Argument Count", 28), 1);
	if (cub_init(argv[1], &game, &memory) == -1)
		return (1);
	while (1)
		stt_loop(&game);
	return (0);
}
