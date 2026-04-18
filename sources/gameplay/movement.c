#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

// Map is defined by uint16_t width height. If x or y are negative,
// the integer wraparound will be guaranteed to be higher than u16_max
static inline
bool	stt_box_collision(t_map *map, float x, float y, float radius)
{
	bool			rvalue;
	const uint32_t	left = (uint32_t)floorf(x - radius);
	const uint32_t	right = (uint32_t)floorf(x + radius);
	const uint32_t	top = (uint32_t)floorf(y - radius);
	const uint32_t	bottom = (uint32_t)floorf(y + radius);

	if (left >= map->width || right >= map->width
		|| top >= map->height || bottom >= map->height)
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
	enemy->cam.dir = vec2_norm(to_player);
	if (mag_dist < MELEE_RANGE)
		enemy->speed.y.f *= 0.9f;
	else
	{
		enemy->speed.y.f += mag_dist * ENEMY_ACCEL;
		enemy->speed.y.f = CLAMP(enemy->speed.y.f, -ENEMY_SPEED, ENEMY_SPEED);
	}
	delta.x.f = enemy->cam.dir.x.f * enemy->speed.y.f * dt;	// forward
	delta.y.f = enemy->cam.dir.y.f * enemy->speed.y.f * dt;
	delta = stt_collision(map, delta, enemy->cam.pos);	// Clamps value to prevent going through walls
	return (delta);
}

static
t_vec2	stt_player_move(t_player *player, t_map *map, float dt)
{
	t_vec2	norm_dir;
	t_vec2	delta;

	player->speed.x.f += player->spd_sideway * PLAYER_ACCEL;	// base accel, dt dependant
	player->speed.y.f += player->spd_forward * PLAYER_ACCEL;
	player->speed.x.f = CLAMP(player->speed.x.f, -player->spd_max, player->spd_max);
	player->speed.y.f = CLAMP(player->speed.y.f, -player->spd_max, player->spd_max);
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
	size_t			i;
	t_vec2			delta;
	t_enemy			*enemy;
	const t_vec2	player_pos = game->player.cam.pos;

	delta = stt_player_move(&game->player, &game->map, dt);
	game->player.cam.pos.x.f += delta.x.f;
	game->player.cam.pos.y.f += delta.y.f;
	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		if (game->enemies[i].health > 0)
		{
			delta = stt_move_toward(&game->map, player_pos, enemy, dt);
			game->enemies[i].cam.pos.x.f += delta.x.f;
			game->enemies[i].cam.pos.y.f += delta.y.f;
		}
		i++;
	}
}
