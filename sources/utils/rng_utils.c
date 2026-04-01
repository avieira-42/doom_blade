#include "cmlx_base.h"
#include "cub_structs.h"
#include "cub_utils.h"

t_vec2	random_valid_pos(t_map *map)
{
	uint16_t	x;
	uint16_t	y;
	t_vec2		pos;

	while (true)
	{
		pos.x.f = 1.0f + ft_randf() * (map->width - 2);	// REVIEW: guarantee map width higher than 3
		pos.y.f = 1.0f + ft_randf() * (map->height - 2);
		x = (uint16_t) floorf(pos.x.f);
		y = (uint16_t) floorf(pos.y.f);
		if (map->tex_index[y * map->width + x] <= 127)
			break ;
	}
	return (pos);
}
