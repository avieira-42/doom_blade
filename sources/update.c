#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include "game_types.h"
#include "game_defines.h"
#include "game_prototypes.h"

void	loop_update(t_game *game)
{
	static long	avg_fps = 0;
	const long	dt = 1 + get_time();

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
	draw_number(game->frame.display, 8, 8, avg_fps, 0xFFFFFFFF);
	cub_play_audio(&game->player, &game->assets.audio, game, dt);
	game->player.state &= ~(size_t)(st_shot);	// Clears the (just X animation)
}
