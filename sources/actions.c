#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx_base.h"
#include "cub_structs.h"
#include "cub_utils.h"

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

t_sheet	*cub_actions(t_player *player, t_map *map, long dt)
{
	size_t	index;
	int		rvalue;
	t_sheet	*sheet;

	if (player->state & st_interacting)
	{
		index = stt_first_neighbor(player->cam.pos, player->cam.dir, map);
		if (map->tex_index[index] == 130)
		{
			map->tex_index[index] &= 127;
			map->state[index] = 0;
		}
		else if (map->tex_index[index] == 2)
		{
			map->tex_index[index] |= 128;
			map->state[index] = 255;
		}
	}
	else if (player->state & st_reloading)
	{
		sheet = &player->hands.reload;
		rvalue = cub_advance_animation(sheet, dt);
		if (rvalue >= 2 && (sheet->index % RELOAD_CYCLE == 0))
			player->ammo++;
		if (rvalue < 4)
			return (sheet);
	}
	else if (player->state & st_shooting)
	{
		sheet = &player->hands.shoot;
		rvalue = cub_advance_animation(sheet, dt);
		if (rvalue < 4)
			return (sheet);
	}
	else
		cub_advance_animation(&player->hands.walk, dt);
	player->state = st_idle;
	return (&player->hands.walk);
}
