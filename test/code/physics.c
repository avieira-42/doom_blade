#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include <math.h>
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

void	character_move(t_game * game, t_player *player, float dt)
{
	player->pos.x += player->vel * player->ori.x * dt;
	player->pos.y += player->vel * player->ori.y * dt;
	player->center.x = player->pos.x + game->map.tile_x * 0.5;
	player->center.y = player->pos.y + game->map.tile_y * 0.5;
}

void	camera_move(t_vecf32 player_pos, t_cam *cam)
{
	//cam->dist += 1 * cam->dist_mod; // might add this to be more eerie
	cam->angle += RADIANS * cam->dir;
	cam->pos.x = player_pos.x + sinf(cam->angle) * cam->dist;
	cam->pos.y = player_pos.y + cosf(cam->angle) * cam->dist;
}

t_vecf32	update_player_dir(t_vecf32 player, t_vecf32 cam)
{
	return (vec_unit(vec_sub(player, cam)));
}
