#include <unistd.h>
#include "../libs/minilibx-linux/mlx.h"
#include "libft_math.h"
#include "types.h"
#include "update.h"

void	screen_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	game->win_ptr = mlx_new_window(game->mlx_ptr, SCREEN_X, SCREEN_Y, "doom_blade");
	game->t0 = time_get();
	time_delta_get(game);
	game->vd = 1 / u_rsqrt(game->map.width * game->map.width
			+ game->map.height * game->map.height);
	game->frame.img = mlx_new_image(game->mlx_ptr, SCREEN_X, SCREEN_Y);
	game->frame.addr = mlx_get_data_addr(
			game->frame.img, &game->frame.bpp, &game->frame.l_len,
				&game->frame.endian);
	if (game->frame.addr == NULL)
		return ;
}

void	player_init(t_game *game)
{
	game->player.dir = (t_vecf32){0.71, 0.71};
	game->player.pos = (t_vecf32){100, 100};
	game->player.ori = (t_vecf32){0, 0};
	game->player.dir_mod = 0;
	game->player.acc = 10;
	game->player.vel = 300;
	game->player.speed = 30;
	game->player.speed_mod = 1;
}

void	cam_init(t_cam *cam, t_player player)
{
	cam->dist = 1;
	cam->dist_mod = 0;
	cam->half_len = 30;
	cam->dir = (t_vecf32){-0.71, 0.71};
	cam->pos = vec_sum(player.pos,
				vec_scalar_mult(player.dir, cam->dist));
}
