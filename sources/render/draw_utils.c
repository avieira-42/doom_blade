#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"

static inline
void	stt_putrgb(t_mat32 *frame, int32_t x, int32_t y, uint32_t color)
{
	if (x < 0 || y < 0 || x >= frame->width || y >= frame->height)
		return ;
	frame->ptr[frame->stride * x + y] = color;
}

// 1) First Render Call, 2) Updated, 4) End
uint8_t	cub_advance_animation(t_sheet *sheet, long dt)
{
	uint8_t	rvalue;

	rvalue = (sheet->index == 0) && (sheet->frame_dt == 0);
	sheet->frame_dt += dt;
	if (sheet->frame_dt < sheet->frame_time)
		return (rvalue);
	rvalue |= (1 << 1);
	sheet->index++;
	if (sheet->index >= sheet->count)	// check if >= or >
	{
		sheet->index = 0;
		rvalue |= (1 << 2);
	}
	sheet->frame_dt = 0;
	return (rvalue);
}

void	draw_line(t_mat32 frame, t_vec2 p0, t_vec2 p1, uint32_t color)
{
	uint32_t	i;
	uint32_t	length;
	t_vec2		pos;
	t_vec2		delta;

	length = ABSMAX(p1.x.i - p0.x.i, p1.y.i - p0.y.i);
	p0.x.i <<= 16;
	p0.y.i <<= 16;
	p1.x.i <<= 16;
	p1.y.i <<= 16;
	length += (length == 0);
	delta.x.i = (p1.x.i - p0.x.i) / (int32_t)length;
	delta.y.i = (p1.y.i - p0.y.i) / (int32_t)length;
	pos.x.i = p0.x.i;
	pos.y.i = p0.y.i;
	i = 0;
	while (i < length)
	{
		stt_putrgb(&frame, pos.x.i >> 16, pos.y.i >> 16, color);
		pos.x.i += delta.x.i;
		pos.y.i += delta.y.i;
		i++;
	}
}

void	draw_circle(t_mat32 frame, t_quad quad)
{
	int32_t		y;
	int32_t		x;
	t_vec2		dst;
	uint32_t	color;

	y = 0;
	while (y <= quad.size.y.i)
	{
		x = 0;
		while (x <= quad.size.x.i)
		{
			dst.x.i = quad.pos.x.i + x;
			dst.y.i = quad.pos.y.i + y;
			color = ((uint32_t)ft_rand() & 0x0F0F0Fu) ^ quad.color;
			if (dst.x.i >= 0 && dst.y.i >= 0
				&& dst.x.i < frame.width && dst.y.i < frame.height
				&& vec2_idist(quad.center, dst) <= quad.radius * quad.radius)
				frame.ptr[dst.x.i * frame.stride + dst.y.i] = color;
			x++;
		}
		y++;
	}
}
