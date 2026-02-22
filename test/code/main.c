#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <X11/keysym.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../libs/minilibx-linux/mlx.h"
#include "types.h"
#include "clean.h"
#include "physics.h"
#include "render.h"
#include "ray_cast.h"
#include "input.h"
#include "parse.h"

void	update(t_game *game)
{
	time_delta_get(game);
	character_move(game, &game->player, game->dt);
	camera_move(game->player.center, &game->player.cam);
	game->player.dir = update_player_dir(game->player.center, game->player.cam.pos);
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
	game->mlx_ptr = mlx_init();
	game->win_ptr = mlx_new_window(game->mlx_ptr, 1920, 1024, "doom_blade");
	game->t0 = time_get();
	//game->map_2d.width = 128;
	//game->map_2d.height = 128;
	game->map_2d.img = mlx_new_image(game->mlx_ptr, 400, 400);
	game->map_2d.addr = mlx_get_data_addr(
			game->map_2d.img, &game->map_2d.bpp, &game->map_2d.l_len, &game->map_2d.endian);
	game->map_3d.img = mlx_new_image(game->mlx_ptr, 1920, 1024);
	game->map_3d.addr = mlx_get_data_addr(
			game->map_3d.img, &game->map_3d.bpp, &game->map_3d.l_len, &game->map_3d.endian);
	game->map_3d.width = 1920;
	game->map_3d.height = 1024;
	time_delta_get(game);
	//game->player = (t_player){.acc = 10, .vel = 300, .ori = (t_vecf32){0, 0}};
	game->player.acc = 10;
	game->player.vel = 300;
	game->player.ori.x = 0;
	game->player.ori.y = 0;
	game->player.dir.x = 0;
	game->player.dir.y = 0;
	game->here = 0;
	game->fov = 90 * RADIANS;
	game->vd = 1024;
	game->rc_size = ray_cast_size(game);
	game->player.cam.dist_mod = 0;
	game->player.cam.angle = 15;
	game->player.cam.dist = game->vd;
	game->player.cam.dir = 0;
	game->player.center.x = 0;
	game->player.center.x = 0;
	camera_move(game->player.pos, &game->player.cam);
	game->camera.acc = 10;
	game->camera.vel = 300;
	game->camera.dir.x = 0;
	game->camera.dir.y = 0;
	game->camera.ori.x = 0;
	game->camera.ori.y = 0;
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
