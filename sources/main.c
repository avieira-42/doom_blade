#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cmlx_base.h"
#include "cmlx_draw.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

int	cmlx_loop(t_game *game)
{
	static long	last_frame = 0;
	const long	dt = get_time();

	if (game->state.paused == false)
	{
		last_frame += dt;
		cub_update_pos(game, (double)dt * 0.000001);	// tmp	
		cub_actions(game);
		cub_update_state(&game->player, &game->assets.audio, game, dt);
		ft_memset(game->frame.display.ptr, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
		ft_memset(game->frame.render.ptr, 0, RENDER_HEIGHT * RENDER_WIDTH * sizeof(uint32_t));
		render_image(game);
		cub_draw_enemies(game, dt);
		// TMP RADAR >>>
		animate_hud(game);
		// <<< TMP RADAR
		cub_draw_hud(game->frame.render, &game->drawbuf, game->player.health, game->assets.blood);
		ft_integer_scaling_t(game->frame.render, game->frame.display, UPSCALING_FACTOR);
		cub_draw_crosshair(game->frame.display.ptr);
		mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame.img, 0, 0);
		cmlx_mousemove(game);
		last_frame = 0;
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
