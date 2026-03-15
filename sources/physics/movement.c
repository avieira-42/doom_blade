#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

// Simple collision check, if player outside of map, pos = last known position
void	cub_update_pos(t_game *game)
{
	t_vec2		norm_dir;
	t_vec2		delta;
	const float forward = !!(game->key & key_w) - !!(game->key & key_s);
	const float right = !!(game->key & key_d) - !!(game->key & key_a);

	game->player.move.speed.x.f += right * (1.0f / 128);
	game->player.move.speed.y.f += forward * (1.0f / 128);
	game->player.move.speed.x.f = ft_clamp(game->player.move.speed.x.f, 1.0f / 32.0f);
	game->player.move.speed.y.f = ft_clamp(game->player.move.speed.y.f, 1.0f / 32.0f);
	if (right == 0.0f)
		game->player.move.speed.x.f *= 0.9f;
	if (forward == 0.0f)
		game->player.move.speed.y.f *= 0.9f;
	norm_dir = vec2_norm(game->player.cam.dir);
	delta.x.f = norm_dir.x.f * game->player.move.speed.y.f;	// forward
	delta.y.f = norm_dir.y.f * game->player.move.speed.y.f;
	delta.x.f -= norm_dir.y.f * game->player.move.speed.x.f; // lateral
	delta.y.f += norm_dir.x.f * game->player.move.speed.x.f;
	game->player.cam.pos.x.f += delta.x.f;
	game->player.cam.pos.y.f += delta.y.f;
}
