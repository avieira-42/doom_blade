#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cmlx_draw.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_cub_is_dead(t_game *game)
{
	if (game->player.health <= 0)
	{
		write(1, "You are dead!\n", 14);
		cub_cleanup(game, NULL);
	}
}

int	cmlx_loop(t_game *game)
{
	static long	avg_fps = 0;
	const long	dt = 1 + get_time();

	if (game->state.paused == false)
	{
		avg_fps = (avg_fps - avg_fps / 8) + 125000 / dt;
		input_handler(game, &game->player);
		cub_update_game(game, dt);
		cub_draw_world(game);
		cub_draw_enemies(game, dt);
		cub_draw_hands(game->frame.render, game, dt);
		cub_draw_blood(game->frame.render, game);
		ft_integer_scaling_t(game->frame.render, game->frame.display, UPSCALING_FACTOR);
		cub_draw_crosshair(game->frame.display.ptr);
		draw_number(game->frame.display, 8, 8, avg_fps);
		mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame.img, 0, 0);
		// stt_cub_is_dead(game);
		cmlx_mousemove(game);
		game->player.state &= ~(size_t) (st_shot);	// Clears the (just X animation)
	}
	return (1);
}

int	main(int argc, char **argv)
{
	static t_game	game;
	static t_memory	memory;

	if (argc != 2)
		return (write(2, "Error\nToo Many Arguments", 24), 1);
	if (cub_init(argv[1], &game, &memory) == -1)
		return (1);
	mlx_loop(game.mlx);
	cub_cleanup(&game, NULL);
	return (0);
}
