#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx_base.h"
#include "cub_structs.h"

size_t	stt_first_neighbor(t_vec2 pos, t_vec2 dir, t_map *map)
{
	int		cx;
	int		cy;
	float	tx;
	float	ty;

	cx = (int)floorf(pos.x.f);
	cy = (int)floorf(pos.y.f);
	if (dir.x.f > 0.0f)
		tx = (cx + 1.0f - pos.x.f) * fabsf(dir.y.f);
	else
		tx = (pos.x.f - cx) * fabsf(dir.y.f);
	if (dir.y.f > 0.0f)
		ty = (cy + 1.0f - pos.y.f) * fabsf(dir.x.f);
	else
		ty = (pos.y.f - cy) * fabsf(dir.x.f);
	if (tx < ty)
		cx += ((dir.x.f > 0.0f) << 1) - 1;
	else
		cy += ((dir.y.f > 0.0f) << 1) - 1;
	return ((uint32_t)cy * map->width + (uint32_t)cx);
}

int	cub_actions(t_game *game)
{
	size_t	index;

	if (game->player.state & st_interacting)
	{
		index = stt_first_neighbor(game->player.cam.pos, game->player.cam.dir, &game->map);
		if (game->map.tex_index[index] == 130)
		{
			game->map.tex_index[index] &= 127;
			game->map.state[index] = 0;
		}
		else if (game->map.tex_index[index] == 2)
		{
			game->map.tex_index[index] |= 128;
			game->map.state[index] = 255;
		}
		game->player.state = st_idle;
	}
	return (0);
}
