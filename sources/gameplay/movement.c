#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

#define PLAYER_RADIUS 0.125f

// TODO: fix diagonal movement being faster

static inline
bool	stt_box_collision(t_map *map, float x, float y, float radius)
{
	bool		rvalue;
	const int	left = (int)floorf(x - radius);
	const int	right = (int)floorf(x + radius);
	const int	top = (int)floorf(y - radius);
	const int	bottom = (int)floorf(y + radius);

	if (left < 0 || right < 0 || left >= map->width || right >= map->width
		|| top < 0 || bottom < 0 || top >= map->height || bottom >= map->height)
		return (1);
	rvalue = (map->tex_index[bottom * map->width + left] > 127)
		|| (map->tex_index[bottom * map->width + right] > 127)
		|| (map->tex_index[top * map->width + left] > 127)
		|| (map->tex_index[top * map->width + right] > 127);
	return (rvalue);
}

static
t_vec2	stt_collision(t_map *map, t_vec2 delta, t_vec2 pos)
{
	if (stt_box_collision(map, pos.x.f + delta.x.f, pos.y.f, PLAYER_RADIUS))
		delta.x.f = 0.0f;
	if (stt_box_collision(map, pos.x.f, pos.y.f + delta.y.f, PLAYER_RADIUS))
		delta.y.f = 0.0f;
	return (delta);
}

static
t_vec2	stt_move_toward(t_map *map, t_vec2 pos, t_enemy *enemy, float dt)
{
	t_vec2	to_player;
	float	mag_dist;
	t_vec2	delta;

	to_player.x.f = pos.x.f - enemy->cam.pos.x.f;
	to_player.y.f = pos.y.f - enemy->cam.pos.y.f;
	mag_dist = to_player.x.f * to_player.x.f + to_player.y.f * to_player.y.f;
	// if (mag_dist >= 10.0f)							// Follows the player only at a specific distance
	// 	return ((t_vec2){.x.f = 0.0f, .y.f = 0.0f});
	enemy->cam.dir = vec2_norm(to_player);
	if (mag_dist < MELEE_RANGE)
		enemy->speed.y.f *= 0.9f;
	else
	{
		enemy->speed.y.f += mag_dist * ENEMY_ACCEL;
		enemy->speed.y.f = ft_absclamp(enemy->speed.y.f, ENEMY_SPEED);
	}
	delta.x.f = enemy->cam.dir.x.f * enemy->speed.y.f * dt;	// forward
	delta.y.f = enemy->cam.dir.y.f * enemy->speed.y.f * dt;
	// delta.x.f -= enemy->cam.dir.y.f * enemy->speed.x.f; // lateral
	// delta.y.f += enemy->cam.dir.x.f * enemy->speed.x.f;
	delta = stt_collision(map, delta, enemy->cam.pos);	// Clamps value to prevent going through walls
	return (delta);
}

static
t_vec2	stt_player_move(t_player *player, t_map *map, float dt)
{
	t_vec2		norm_dir;
	t_vec2		delta;

	player->speed.x.f += player->spd_sideway * PLAYER_ACCEL;	// base accel, dt dependant
	player->speed.y.f += player->spd_forward * PLAYER_ACCEL;
	player->speed.x.f = ft_absclamp(player->speed.x.f, player->spd_max);
	player->speed.y.f = ft_absclamp(player->speed.y.f, player->spd_max);
	if (player->spd_sideway == 0.0f)
		player->speed.x.f *= 0.9f;
	if (player->spd_forward == 0.0f)
		player->speed.y.f *= 0.9f;
	norm_dir = vec2_norm(player->cam.dir);
	if (player->spd_forward != 0.0f && player->spd_sideway != 0.0f)
	{
		norm_dir.x.f *= 0.70710678118654752440;	// fixes speed boost on diagonals
		norm_dir.y.f *= 0.70710678118654752440;
	}
	delta.x.f = norm_dir.x.f * player->speed.y.f * dt;	// forward
	delta.y.f = norm_dir.y.f * player->speed.y.f * dt;
	delta.x.f -= norm_dir.y.f * player->speed.x.f * dt; // lateral
	delta.y.f += norm_dir.x.f * player->speed.x.f * dt;
	delta = stt_collision(map, delta, player->cam.pos);	// Clamps value to prevent going through walls
	return (delta);
}

void	cub_update_pos(t_game *game, float dt)
{
	size_t	i;
	t_vec2	delta;

	delta = stt_player_move(&game->player, &game->map, dt);
	game->player.cam.pos.x.f += delta.x.f;
	game->player.cam.pos.y.f += delta.y.f;
	i = 0;
	while (i < NUM_ENEMIES)
	{
		if (game->enemies[i].health > 0)
		{
			delta = stt_move_toward(&game->map, game->player.cam.pos, game->enemies + i, dt);
			game->enemies[i].cam.pos.x.f += delta.x.f;
			game->enemies[i].cam.pos.y.f += delta.y.f;
		}
		i++;
	}
}
