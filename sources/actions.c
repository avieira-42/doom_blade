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
	tx = FLT_MAX;
	ty = FLT_MAX;
	if (dir.x.f > 0.0f)
		tx = ((cx + 1.0f) - pos.x.f) / dir.x.f;
	else if (dir.x.f < 0.0f)
		tx = (cx - pos.x.f) / dir.x.f;
	if (dir.y.f > 0.0f)
		ty = ((cy + 1.0f) - pos.y.f) / dir.y.f;
	else if (dir.y.f < 0.0f)
		ty = (cy - pos.y.f) / dir.y.f;
	if (tx < ty)
		cx += (dir.x.f > 0.0f ? 1 : -1);
	else
		cy += (dir.y.f > 0.0f ? 1 : -1);
	return (cy * map->width + cx);
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
