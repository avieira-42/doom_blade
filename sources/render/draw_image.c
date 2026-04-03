#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	quad_draw(t_game *game, t_quad q)
{
	t_vec2	limit;
	int32_t	y;
	int32_t	x;
	int32_t	dist;

	limit.x.i = q.pos.x.i + q.size.x.i;
	limit.y.i = q.pos.y.i + q.size.y.i;
	y = q.pos.y.i;
	while (y <= limit.y.i)
	{
		x = q.pos.x.i;
		while (x <= limit.x.i)
		{
			dist = vec2_idist(q.map_center, (t_vec2){.x.i = x, .y.i = y});
			if (dist <= q.bound * q.bound)
				pixel_swap(game->frame.render, x, y, q.color);
			x++;
		}
		y++;
	}
}

void	pixel_swap(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
	uint32_t	*dst;

	if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
		return ;
	dst = frame.ptr + x * frame.stride + y;
	if (color == 0x000000)
	{
		if (*dst == 2693401)
			color = 0xd5213f;
		else if (*dst == 2299157)
			color = 0xd72a55;
	}
	else if (color == 0xFFFFFF)
	{
		if (*dst == 2693401)
			color = 0xe6c84b;
	}
	*dst = color;
}

void	pixel_put(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
	uint32_t	*dst;

	if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
		return ;
	dst = frame.ptr + x * frame.stride + y;
	*dst = color;
}

void	cub_draw_texture(t_mat32 frame, t_mat32 image,
		size_t x_corner, size_t y_corner)
{
	size_t		x;
	size_t		y;
	uint32_t	color;

	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x * image.stride + y];
			if (color != 2228223 && color != 1441791)	// REVIEW: colors are hex coded normally
				pixel_put(frame, x + x_corner, y + y_corner, color);
			x++;
		}
		y++;
	}
}
