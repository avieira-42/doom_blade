#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

#define PLAYER_RADIUS 0.125f

// TODO: fix diagonal movement being faster

static inline
bool	stt_box_collision(t_mat8 *map, float x, float y, float radius)
{
	bool		rvalue;
	const int	left = (int)floorf(x - radius);
	const int	right = (int)floorf(x + radius);
	const int	top = (int)floorf(y - radius);
	const int	bottom = (int)floorf(y + radius);

	if (left < 0 || right < 0 || left >= map->width || right >= map->width
		|| top < 0 || bottom < 0 || top >= map->height || bottom >= map->height)
		return (1);
	rvalue = map->ptr[bottom * map->stride + left]
		|| map->ptr[bottom * map->stride + right]
		|| map->ptr[top * map->stride + left]
		|| map->ptr[top * map->stride + right];
	return (rvalue);
}

static
t_vec2	stt_collision(t_mat8 *map, t_vec2 delta, t_vec2 pos)
{
	if (stt_box_collision(map, pos.x.f + delta.x.f, pos.y.f, PLAYER_RADIUS))
		delta.x.f = 0.0f;
	if (stt_box_collision(map, pos.x.f, pos.y.f + delta.y.f, PLAYER_RADIUS))
		delta.y.f = 0.0f;
	return (delta);
}

static
t_vec2	stt_move_toward(t_mat8 *map, t_vec2 pos, t_entity *enemy, float dt)
{
	t_vec2	to_player;
	float	mag_dist;
	t_vec2	delta;

	to_player.x.f = pos.x.f - enemy->cam.pos.x.f;
	to_player.y.f = pos.y.f - enemy->cam.pos.y.f;
	enemy->cam.dir = vec2_norm(to_player);
	mag_dist = to_player.x.f * to_player.x.f + to_player.y.f * to_player.y.f;
	if (mag_dist < MELEE_RANGE)
		enemy->move.speed.y.f *= 0.9f;
	else
	{
		enemy->move.speed.y.f += mag_dist * dt * ENEMY_ACCEL;
		enemy->move.speed.y.f = ft_absclamp(enemy->move.speed.y.f, ENEMY_SPEED * dt);
	}
	delta.x.f = enemy->cam.dir.x.f * enemy->move.speed.y.f;	// forward
	delta.y.f = enemy->cam.dir.y.f * enemy->move.speed.y.f;
	// delta.x.f -= enemy->cam.dir.y.f * enemy->move.speed.x.f; // lateral
	// delta.y.f += enemy->cam.dir.x.f * enemy->move.speed.x.f;
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
	const float	move_speed = PLAYER_SPEED * dt * (1.0f + SPRINT_SPEED * !!(game->state.key & key_shift));

	game->player.move.speed.x.f += right * dt * PLAYER_ACCEL;	// base accel, dt dependant
	game->player.move.speed.y.f += forward * dt * PLAYER_ACCEL;
	game->player.move.speed.x.f = ft_absclamp(game->player.move.speed.x.f, move_speed);
	game->player.move.speed.y.f = ft_absclamp(game->player.move.speed.y.f, move_speed);
	if (right == 0.0f)
		game->player.move.speed.x.f *= 0.9f;
	if (forward == 0.0f)
		game->player.move.speed.y.f *= 0.9f;
	norm_dir = vec2_norm(game->player.cam.dir);
	if (forward != 0.0f && right != 0.0f)
	{
		norm_dir.x.f *= 0.70710678118654752440;	// fixes speed boost on diagonals
		norm_dir.y.f *= 0.70710678118654752440;
	}
	delta.x.f = norm_dir.x.f * game->player.move.speed.y.f;	// forward
	delta.y.f = norm_dir.y.f * game->player.move.speed.y.f;
	delta.x.f -= norm_dir.y.f * game->player.move.speed.x.f; // lateral
	delta.y.f += norm_dir.x.f * game->player.move.speed.x.f;
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
