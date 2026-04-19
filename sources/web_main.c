#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <emscripten.h>

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "game_defines.h"
#include "game_types.h"
#include "game_prototypes.h"

static
void	stt_loop(void *arg)
{
	t_game		*game;

	game = (t_game *)arg;
	if (game->state.paused == false)
	{
		loop_update(game);
		SDL_UpdateTexture (game->texture, NULL, game->frame.img->pixels, game->frame.img->pitch);
		SDL_RenderClear(game->renderer);
		SDL_RenderTexture(game->renderer, game->texture, NULL, NULL);
		SDL_RenderPresent(game->renderer);
	}
}

int	main(int argc, char **argv)
{
	const char		*map;
	static t_game	game;
	static t_memory	memory;

	map = "assets/maps/castle_porto.cub";
	if (argc >= 2)
		map = argv[1];
	printf("INIT START\n");
	if (cub_init(map, &game, &memory) == -1)
		return (1);
	printf("INIT OK\n");
	game.renderer = SDL_CreateRenderer(game.window, NULL);
	if (game.renderer == NULL)
		return (write(2, SDL_GetError(), SDL_strlen(SDL_GetError())), write(2, "\n", 1), 1);
	game.texture = SDL_CreateTexture(
		game.renderer,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		S_WIDTH,
		S_HEIGHT
	);
	if (game.texture == NULL)
		return (write(2, SDL_GetError(), SDL_strlen(SDL_GetError())), write(2, "\n", 1), 1);
	emscripten_set_main_loop_arg(stt_loop, &game, 0, true);
	return (0);
}
