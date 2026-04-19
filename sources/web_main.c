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
void	stt_cub_is_dead(t_game *game)
{
	if (game->player.health <= 0)
	{
		write(1, "You are dead!\n", 14);
		cub_cleanup(game, NULL);
	}
}

static
void	stt_loop(void *arg)
{
	t_game		*game;
	static long	avg_fps = 0;
	const long	dt = 1 + get_time();

	game = (t_game *)arg;
	if (game->state.paused == false)
	{
		avg_fps = (avg_fps - avg_fps / 8) + 125000 / dt;
		sdl_input_update(game);
		input_handler(game, &game->player);
		cub_update_game(game, dt);
		cub_draw_world(game);
		cub_draw_enemies(game, dt);
		cub_draw_viewmodel(game->frame.render, &game->player, game, dt);
		cub_draw_blood(game->frame.render, game);
		ft_integer_scaling_t(game->frame.render, game->frame.display, UPSCALE);
		cub_draw_crosshair(game->frame.display.ptr);
		draw_number(game->frame.display, 8, 8, avg_fps);
		SDL_UpdateTexture (game->texture, NULL, game->frame.img->pixels, game->frame.img->pitch);
		SDL_RenderClear(game->renderer);
		SDL_RenderTexture(game->renderer, game->texture, NULL, NULL);
		SDL_RenderPresent(game->renderer);
		stt_cub_is_dead(game);
		cub_play_audio(&game->player, &game->assets.audio, game, dt);
		game->player.state &= ~(size_t)st_shot;
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
