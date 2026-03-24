#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_relative(t_mat32 frame, t_rayhit *rays, t_entity *player, t_entity *enemy);

int	cmlx_loop(t_game *game)
{
	static long	last_frame = 0;
	const long	dt = get_time();

	last_frame += dt;
	if (last_frame > 10000)
	{
		cub_update_pos(game);
		last_frame = 0;
	}
<<<<<<< HEAD
	ft_memset(game->frame.display.ptr, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
	ft_memset(game->frame.render.ptr, 0, RENDER_HEIGHT * RENDER_WIDTH * sizeof(uint32_t));
	render_image(&game->player.cam, &game->map, game->blocks, &game->frame);
	cub_draw_relative(game->frame.render, game->frame.rays, &game->player, game->enemies);
	ft_integer_scaling_t(game->frame.render, game->frame.display, UPSCALING_FACTOR);
	mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame.img, 0, 0);
	cmlx_mousemove(game);
=======
    input_handler(game);
	if (game->pause == false)
	{
		//time_delta_get(game);
		ft_memset(game->display_frame.ptr, 0, SCREEN_HEIGHT * SCREEN_WIDTH * sizeof(uint32_t));
		ft_memset(game->render_frame.ptr, 0, RENDER_HEIGHT * RENDER_WIDTH * sizeof(uint32_t));
		render_image(&game->player.cam, &game->map, game->blocks, game->render_frame);
		ft_integer_scaling_t(game->render_frame, game->display_frame, UPSCALING_FACTOR);
		animate_hud(game);
		// OLD >>>>>>>>>>
		// HUD ANIMATION CLEAR >>>
		//window_clear(&game->frame, 0x000000);
		// <<< HUD ANIMATION CLEAR
		// HUD ANIMATION >>>
		//animate(game);
		// <<<<<< OLD
		//mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame.image, 0, 0);
		// <<< HUD ANIMATION
		mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->img, 0, 0);
		cmlx_mousemove(game);
	}
>>>>>>> soeiro
	return (1);
}

int	main(int argc, char **argv)
{
	static t_game	game;
	static t_memory	memory;

	if (argc != 2)
		return (1);
	if (cub_init(argv[1], &game, &memory) == -1)
		return (1);
	game.enemies[0].cam.pos = game.player.cam.pos;
	mlx_loop(game.mlx);
}
