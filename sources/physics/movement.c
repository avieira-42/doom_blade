#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

#define PLAYER_RADIUS 0.125f

// TODO: fix diagonal movement being faster

static inline
bool	stt_box_collision(t_mat8 *map, t_vec2 pos, float radius)
{
	bool		rvalue;
	const int	left = (int)floorf(pos.x.f - radius);
	const int	right = (int)floorf(pos.x.f + radius);
	const int	top = (int)floorf(pos.y.f - radius);
	const int	bottom = (int)floorf(pos.y.f + radius);

	if (left < 0 || right < 0 || left >= map->width || right >= map->width
		|| top < 0 || bottom < 0 || top >= map->height || bottom >= map->height)
		return (1);
	rvalue = map->ptr[bottom * map->stride + left]
		|| map->ptr[bottom * map->stride + right]
		|| map->ptr[top * map->stride + left]
		|| map->ptr[top * map->stride + right];
	if (rvalue == 1)
		return (rvalue);	// Debugging purposes
	return (rvalue);
}

static
t_vec2	stt_collision(t_mat8 *map, t_vec2 delta, t_entity *entity)
{
	t_vec2	pos;

	pos = entity->cam.pos;
	pos.x.f += delta.x.f;
	if (stt_box_collision(map, pos, PLAYER_RADIUS))
		delta.x.f = 0.0f;
	pos = entity->cam.pos;
	pos.y.f += delta.y.f;
	if (stt_box_collision(map, pos, PLAYER_RADIUS))
		delta.y.f = 0.0f;
	return (delta);
}

void	cub_update_pos(t_game *game)
{
	t_vec2		norm_dir;
	t_vec2		delta;
	const float forward = !!(game->state.key & key_w) - !!(game->state.key & key_s);
	const float right = !!(game->state.key & key_d) - !!(game->state.key & key_a);
	const float	move_speed = MOVE_SPEED * (1.0f + SPRINT_SPEED * !!(game->state.key & key_shift));

	game->player.move.speed.x.f += right * (1.0f / 128);
	game->player.move.speed.y.f += forward * (1.0f / 128);
	game->player.move.speed.x.f = ft_absclamp(game->player.move.speed.x.f, move_speed);
	game->player.move.speed.y.f = ft_absclamp(game->player.move.speed.y.f, move_speed);
	if (right == 0.0f)
		game->player.move.speed.x.f *= 0.9f;
	if (forward == 0.0f)
		game->player.move.speed.y.f *= 0.9f;
	norm_dir = vec2_norm(game->player.cam.dir);
	delta.x.f = norm_dir.x.f * game->player.move.speed.y.f;	// forward
	delta.y.f = norm_dir.y.f * game->player.move.speed.y.f;
	delta.x.f -= norm_dir.y.f * game->player.move.speed.x.f; // lateral
	delta.y.f += norm_dir.x.f * game->player.move.speed.x.f;
	delta = stt_collision(&game->map, delta, &game->player);
	game->player.cam.pos.x.f += delta.x.f;
	game->player.cam.pos.y.f += delta.y.f;
}
