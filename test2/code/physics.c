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

void	player_move(t_player *player, float dt)
{
	player->pos.x += player->vel * player->ori.x * dt;
	player->pos.y += player->vel * player->ori.y * dt;
	if (player->dir_mod == -1)
		player->dir = vec_rotate(player->dir, 1, RIGHT);
	if (player->dir_mod == 1)
		player->dir = vec_rotate(player->dir, 1, LEFT);
}

#include <stdio.h>
void	camera_move(t_player player, t_cam *cam)
{
	cam->dist += cam->dist_mod;
	cam->pos = vec_sum(player.pos, vec_scalar_mult(player.dir, cam->dist));
	printf("x: %f\ny: %f\n", player.dir.x, player.dir.y);
	if (player.dir_mod == -1)
		cam->dir = vec_rotate(cam->dir, 1, RIGHT);
	if (player.dir_mod == 1)
		cam->dir = vec_rotate(cam->dir, 1, LEFT);
}
