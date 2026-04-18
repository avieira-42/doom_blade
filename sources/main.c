#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "game_types.h"
#include "game_types.h"
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

void	stt_loop(t_game *game)
{
	static long	avg_fps = 0;
	const long	dt = 1 + get_time();
	SDL_Surface	*window_surface = SDL_GetWindowSurface(game->window);

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
		SDL_BlitSurface(game->frame.img, NULL, window_surface, NULL);
		SDL_UpdateWindowSurface(game->window);
		stt_cub_is_dead(game);
		cub_play_audio(&game->player, &game->assets.audio, game, dt);
		game->player.state &= ~(size_t)(st_shot);	// Clears the (just X animation)
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
