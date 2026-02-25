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
#include "ray_cast.h"

void	update(t_game *game)
{
	time_delta_get(game);
	player_move(&game->player, game->dt);
	camera_move(game->player, &game->cam);
}

void	render(t_game *game)
{
	window_clear(game, 0x000000);
	minimap_draw(game);

	// debug
	//ray_cast(game, game->player, game->player.dir);
	//t_vecf32 line = vec_dir(game->cam.pos, game->player.pos);
	//printf("P_DIR\nx: %f\ny: %f\n\n", game->player.dir.x, game->player.dir.y);
	//printf("P_LINE\nx: %f\ny: %f\n\n", line.x, game->player.dir.y);
	//line_draw_bresenham(game->player.pos, game->cam.pos, game, 0xFF0000);
	//line_draw_bresenham(game->cam.pos, vec_sum(game->cam.pos, vec_scalar_mult(game->cam.dir, game->cam.half_len)), game, 0x00FF00);
	//line_draw_bresenham(game->cam.pos, vec_sum(game->cam.pos, vec_scalar_mult(game->cam.dir, game->cam.half_len * -1)), game, 0x00FF00);
	// debug

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
	cam_init(game);

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
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game);
	mlx_hook(game.win_ptr, 03, (1L << 1), key_released, &game);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx_ptr, game_loop, &game);
	mlx_loop(game.mlx_ptr);
}
