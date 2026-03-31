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
	rvalue = (map->tex_index[bottom * map->width + left] & 128)
		|| (map->tex_index[bottom * map->width + right] & 128)
		|| (map->tex_index[top * map->width + left] & 128)
		|| (map->tex_index[top * map->width + right] & 128);
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
	enemy->cam.dir = vec2_norm(to_player);				// TODO: Review if cam dir needs to change when stopping
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
t_vec2	stt_player_move(t_game *game, float dt)
{
	t_vec2		norm_dir;
	t_vec2		delta;
	const float	forward = !!(game->state.key & key_w) - !!(game->state.key & key_s);
	const float	right = !!(game->state.key & key_d) - !!(game->state.key & key_a);
	const float	move_speed = PLAYER_SPEED * (1.0f + SPRINT_SPEED * !!(game->state.key & key_shift));

	game->player.speed.x.f += right * PLAYER_ACCEL;	// base accel, dt dependant
	game->player.speed.y.f += forward * PLAYER_ACCEL;
	game->player.speed.x.f = ft_absclamp(game->player.speed.x.f, move_speed);
	game->player.speed.y.f = ft_absclamp(game->player.speed.y.f, move_speed);
	if (right == 0.0f)
		game->player.speed.x.f *= 0.9f;
	if (forward == 0.0f)
		game->player.speed.y.f *= 0.9f;
	norm_dir = vec2_norm(game->player.cam.dir);
	if (forward != 0.0f && right != 0.0f)
	{
		norm_dir.x.f *= 0.70710678118654752440;	// fixes speed boost on diagonals
		norm_dir.y.f *= 0.70710678118654752440;
	}
	delta.x.f = norm_dir.x.f * game->player.speed.y.f * dt;	// forward
	delta.y.f = norm_dir.y.f * game->player.speed.y.f * dt;
	delta.x.f -= norm_dir.y.f * game->player.speed.x.f * dt; // lateral
	delta.y.f += norm_dir.x.f * game->player.speed.x.f * dt;
	delta = stt_collision(&game->map, delta, game->player.cam.pos);	// Clamps value to prevent going through walls
	return (delta);
}

void	cub_update_pos(t_game *game, float dt)
{
	size_t	i;
	t_vec2	delta;

	delta = stt_player_move(game, dt);
	game->player.cam.pos.x.f += delta.x.f;
	game->player.cam.pos.y.f += delta.y.f;
	i = 0;
	while (i < NUM_ENEMIES)
	{
		delta = stt_move_toward(&game->map, game->player.cam.pos, game->enemies + i, dt);
		game->enemies[i].cam.pos.x.f += delta.x.f;
		game->enemies[i].cam.pos.y.f += delta.y.f;
		i++;
	}
}
