#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "game_defines.h"
#include "game_types.h"
#include "game_prototypes.h"

static
void	stt_update_entities(t_game *game, long dt)
{
	size_t	i;
	t_enemy	*enemy;

	if (game->player.health <= 0)
		exit_log(1, "You are dead!\n", 0);
	cub_update_pos(game, (double)dt * 0.000001);
	update_player_state(&game->player, &game->map, dt);
	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		update_enemy_state(&game->map, &game->player, enemy, dt);
		i++;
	}
}

void	game_update(t_game *game)
{
	static long	avg_fps = 0;
	const long	dt = 1 + get_time();

	avg_fps = (avg_fps - avg_fps / 8) + 125000 / dt;
	sdl_input_update(game);
	input_handler(game, &game->player);
	stt_update_entities(game, dt);
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
