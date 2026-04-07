#include <iso646.h>
#include <stdint.h>
#include "cmlx_base.h"
#include "cub_structs.h"
#include "cub_utils.h"

static inline
void	stt_putrgb(t_mat32 *frame, int32_t x, int32_t y, uint32_t color)
{
	if (x < 0 || y < 0 || x >= frame->width || y >= frame->height)
		return ;
	frame->ptr[frame->stride * x + y] = color;
}

void	draw_line(t_mat32 frame, t_vec2 p0, t_vec2 p1, uint32_t color)
{
	uint32_t	i;
	uint32_t	length;
	t_vec2		pos;
	t_vec2		delta;

	length = ft_iabsmax(p1.x.i - p0.x.i, p1.y.i - p0.y.i);
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
