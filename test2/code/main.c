#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../libs/minilibx-linux/mlx.h"
#include "types.h"
#include "clean.h"
#include "update.h"
#include "render.h"
#include "input.h"
#include "parse.h"
#include "init.h"

t_vecf32	collision = (t_vecf32){0,0};

void	update(t_game *game)
{
	time_delta_get(game);
	player_move(game, &game->player, &game->cam, game->dt, &collision);
	camera_move(&game->player, &game->cam);
	mlx_mouse_move(game->mlx_ptr, game->win_ptr, SCREEN_X / 2, SCREEN_Y / 2);
}

void	render(t_game *game)
{
	window_clear(game, 0x000000);
	space_render(game);
	if (collision.x != 0 && collision.y != 0)
		quad_draw((t_vecf32){collision.x * game->map.tile_x, collision.y *game->map.tile_y},
				game, 0xffffff,
				(t_vecf32){game->map.tile_x, game->map.tile_y});
	mlx_put_image_to_window(game->mlx_ptr, game->win_ptr,
			game->frame.img, 0, 0);
}

int	game_loop(void *arg)
{
	t_game *game;

	game = (t_game *)arg;
	update(game);
	render(game);
	return (1);
}

void	game_init(t_game *game)
{
	screen_init(game);
	time_delta_get(game);
	player_init(game);
	cam_init(&game->cam, game->player);

	// DEBUG
	game->here = 0;
	game->start = 0;
}

int	main(int argc, char **argv)
{
	if (argc != 2)
		return (1);
	t_game	game;

	if (parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_mouse_hide(game.mlx_ptr, game.win_ptr);
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game);
	mlx_hook(game.win_ptr, 03, (1L << 1), key_released, &game);
	mlx_hook(game.win_ptr, 04, (1L << 2), mouse_pressed, &game);
	mlx_hook(game.win_ptr, 05, (1L << 3), mouse_released, &game);
	mlx_hook(game.win_ptr, 06, (1L << 6), mouse_move, &game);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_loop(game.mlx_ptr);
}
