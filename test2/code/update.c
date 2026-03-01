#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include "types.h"
#include "clean.h"

uint64_t 	time_get()
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		free_displays();
	return ((uint64_t)(tv.tv_sec * 1000000) + (tv.tv_usec));
}

void	time_delta_get(t_game *game)
{
	game->dt = (time_get() - game->t0) / 1000000.0f;
	game->t0 = time_get();
}

void		player_move(t_player *player, t_cam cam, float dt)
{
	player->pos = vec_sum(player->pos,
			vec_scalar_mult(player->dir, player->ori.y * player->speed
								* dt * player->speed_mod));
	player->pos = vec_sum(player->pos,
			vec_scalar_mult(cam.dir, player->ori.x * player->speed * dt));
	if (player->mouse_mov.x == 0)
	{
		if (player->dir_mod == -1)
			player->dir = vec_rotate(player->dir, 1, RIGHT);
		if (player->dir_mod == 1)
			player->dir = vec_rotate(player->dir, 1, LEFT);
	}
	else
	{
		if (player->mouse_mov.x > 0.)
			player->dir = vec_rotate(player->dir, player->mouse_mov.x, RIGHT);
		if (player->mouse_mov.x < 0.)
			player->dir = vec_rotate(player->dir, player->mouse_mov.x * -1, LEFT);
	}
	//printf("mouse_mov.x: %f\n", player->mouse_mov.x);
}

void	camera_move(t_player player, t_cam *cam)
{
	if (player.mouse_mov.x == 0)
	{
		if (player.dir_mod == -1)
			cam->dir = vec_rotate(cam->dir, 1, RIGHT);
		if (player.dir_mod == 1)
			cam->dir = vec_rotate(cam->dir, 1, LEFT);
	}
	else
	{
		if (player.mouse_mov.x > 0.)
			cam->dir = vec_rotate(cam->dir, player.mouse_mov.x, RIGHT);
		if (player.mouse_mov.x < 0.)
			cam->dir = vec_rotate(cam->dir, player.mouse_mov.x * -1, LEFT);
	}
	cam->dist += 0.1 * cam->dist_mod;
	if (cam->dist <= 1)
		cam->dist = 1;
	cam->pos = vec_sum(player.pos, vec_scalar_mult(player.dir, cam->dist));
}
