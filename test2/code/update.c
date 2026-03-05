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

bool	collision_check(t_game *game, t_vecf32 new_pos)
{
	if (game->map.grid[(int)new_pos.y][(int)new_pos.x] == '1')
		return (true);
	return (false);
}

void		player_move(t_game *game, t_player *player, t_cam *cam, float dt)
{
	t_vecf32	new_pos;

	new_pos = vec_sum(player->pos,
			vec_scalar_mult(player->dir, player->ori.y * player->speed
								* dt * player->speed_mod));
	new_pos = vec_sum(new_pos,
			vec_scalar_mult(cam->dir, player->ori.x * player->speed * dt));
	if (collision_check(game, new_pos) == false)
		player->pos = new_pos;
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
}

void	camera_move(t_player *player, t_cam *cam)
{
	if (player->mouse_mov.x == 0)
	{
		if (player->dir_mod == -1)
			cam->dir = vec_rotate(cam->dir, 1, RIGHT);
		if (player->dir_mod == 1)
			cam->dir = vec_rotate(cam->dir, 1, LEFT);
	}
	else
	{
		if (player->mouse_mov.x > 0.)
			cam->dir = vec_rotate(cam->dir, player->mouse_mov.x, RIGHT);
		if (player->mouse_mov.x < 0.)
			cam->dir = vec_rotate(cam->dir, player->mouse_mov.x * -1, LEFT);
	}
	cam->dist += 0.1 * cam->dist_mod;
	if (cam->dist <= 1)
		cam->dist = 1;
	cam->pos = vec_sum(player->pos, vec_scalar_mult(player->dir, cam->dist));
}
