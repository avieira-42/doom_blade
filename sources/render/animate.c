#include "mlx_int.h"
#include "cub_types.h"

/*void	frame_pixel_put(t_img *frame, int32_t x, int32_t y, int32_t color)
{
	char	*dst;
	if (x < 0 || x >= SCREEN_WIDTH || y < 0 || y >= SCREEN_HEIGHT)
		return ;
	dst = frame->data + (y * frame->size_line + x * frame->bpp / 8);
	*(unsigned int *)dst = color;
}

void	window_clear(t_img *frame, int32_t color)
{
	size_t	i;
	size_t	j;

	i = 0;
	while (i < SCREEN_HEIGHT)
	{
		j = 0;
		while (j < SCREEN_WIDTH)
		{
			frame_pixel_put(frame, j, i, color);
			j++;
		}
		i++;
	}
}

static
int32_t	stt_get_color(t_img *image, int32_t x, int32_t y)
{
	int32_t	*color;

	if (x < 0 || x >= image->width || y < 0 || y > image->height)
		return (0);
	color = (int32_t *) (image->data + x * (image->bpp / 8) + y * image->size_line);
	return (*color);
}

void	draw_texture(t_img *frame, t_img *image, t_vec2 pos, float scale)
{
	int32_t	x;
	int32_t	y;
	int32_t	color;	

	x = 0;
	while (x < image->width * scale)
	{
		y = 0;
		while (y < image->height * scale)
		{
			color = stt_get_color(image, x / scale, y / scale);
			if (color != 2228223 && color != 1441791)
				frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
			y++;
		}
		x++;
	}
}*/

void	sprite_sheet_animate(t_img *frame, t_anim *animation, t_vec2 pos, float scale)
{
	int32_t	i;

	i = animation->iterator;
	animation->counter += *animation->dt * 30;
	if (animation->counter >= animation->loops_per_sprite)
	{
		animation->counter = 0;
		animation->iterator++;
		if (animation->iterator== animation->size)
		{
			animation->iterator= 0;
			animation->end = true;
		}
	}
	cub_draw_image(frame, &animation->sheet[i], pos, scale);
}
