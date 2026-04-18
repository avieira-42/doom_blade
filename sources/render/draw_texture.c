#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "game_prototypes.h"

static inline
void	stt_putrgb(t_mat32 *frame, uint32_t x, uint32_t y, uint32_t color)
{
	if (x >= frame->width || y >= frame->height)
		return ;
	frame->ptr[frame->stride * x + y] = color;
}

// TODO: putrgb shouldnt be needed here
void	cub_draw_texture(t_mat32 frame, t_mat32 image, uint32_t xc, uint32_t yc)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;

	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x * image.stride + y];
			if (color != 0)
				stt_putrgb(&frame, x + xc, y + yc, color);
			x++;
		}
		y++;
	}
}

void	cub_draw_sheet(t_mat32 frame, const t_sheet *sheet, uint32_t xc, uint32_t yc)
{
	uint32_t	x;
	uint32_t	y;
	uint32_t	color;
	t_mat32		image;

	image = sheet->texture;
	image.ptr += sheet->frame_size * sheet->index;
	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x * image.stride + y];
			if (color != 0)
				stt_putrgb(&frame, x + xc, y + yc, color);
			x++;
		}
		y++;
	}
}
