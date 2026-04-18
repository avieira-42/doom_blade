#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <sys/time.h>
#include "cub_utils.h"

t_sides	cub_center_clip(t_mat32 frame, t_vec2 draw_pos, t_vec2 new_size)
{
	t_sides	bounds;

	bounds.x = draw_pos.x.i - new_size.x.i / 2;
	bounds.y = draw_pos.y.i - new_size.y.i / 2;
	bounds.left = CLAMP(bounds.x, 0, frame.width);
	bounds.right = CLAMP(draw_pos.x.i + new_size.x.i / 2, 0, frame.width);
	bounds.top = CLAMP(bounds.y, 0, frame.height);
	bounds.bottom = CLAMP(draw_pos.y.i + new_size.y.i / 2, 0, frame.height);
	return (bounds);
}

t_vec2	random_valid_pos(t_map *map)
{
	uint16_t	x;
	uint16_t	y;
	t_vec2		pos;

	while (true)
	{
		pos.x.f = 1.5f + floorf(ft_randf() * (map->width - 2));
		pos.y.f = 1.5f + floorf(ft_randf() * (map->height - 2));
		x = (uint16_t) floorf(pos.x.f);
		y = (uint16_t) floorf(pos.y.f);
		if (map->tex_index[y * map->width + x] <= 127)
			break ;
	}
	return (pos);
}

size_t	cub_first_neighbour(t_vec2 pos, t_vec2 dir, uint32_t stride)
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
	return ((uint32_t)cy * stride + (uint32_t)cx);
}

long	get_time(void)
{
	static long		prev_time = 0;
	long			dt;
	struct timeval	now;

	dt = -prev_time;
	gettimeofday(&now, NULL);
	prev_time = now.tv_sec * 1000000 + now.tv_usec;
	dt += prev_time;
	return (dt);
}

long	get_time_v2(void)
{
	static long		prev_time[8];	// Stores the result of the last N measurements
	static size_t	index = 0;
	long			dt;
	struct timeval	now;

	gettimeofday(&now, NULL);
	dt = -prev_time[index];
	index = (index + 1) % 8;
	prev_time[index] = now.tv_sec * 1000000 + now.tv_usec;
	dt += prev_time[index];
	return (dt);
}
