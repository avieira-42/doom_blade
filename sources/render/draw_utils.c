#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_crosshair(uint32_t *ptr)
{
	size_t	index;

	index = (SCREEN_HEIGHT / 2 - 1) * SCREEN_WIDTH + (SCREEN_WIDTH / 2 - 1);
	ptr[index] = 0;
	ptr[index + 1] = 0;
	ptr[index + 2] = 0;
	ptr[index + 3] = 0;
	index += SCREEN_WIDTH;
	ptr[index] = 0;
	ptr[index + 1] = 0xFFFFFFFF;
	ptr[index + 2] = 0xFFFFFFFF;
	ptr[index + 3] = 0;
	index += SCREEN_WIDTH;
	ptr[index] = 0;
	ptr[index + 1] = 0xFFFFFFFF;
	ptr[index + 2] = 0xFFFFFFFF;
	ptr[index + 3] = 0;
	index += SCREEN_WIDTH;
	ptr[index] = 0;
	ptr[index + 1] = 0;
	ptr[index + 2] = 0;
	ptr[index + 3] = 0;
}

// Animation system pipeline:
// 1) Input is processed, and player states are determined (shooting, reloading, etc). This also changes the animation sheet to be loaded.
// 2) The animation render takes care of the frame advancing, and returns whether the animation is done or not
// 3) The return is processed, and player states are determined accordingly.
// Example: 
// 1) R is pressed, and if the player isnt shooting and is below max ammo, he goes into the reloading state. Reloading hands are loaded into hands
// 2) Animation gets rendered, and returns that it was the first rendered frame of a reload cycle
// 3) Given that return, ammo count is increased. Suppose that it returned end of animation, then the player state gets changed to idle and idle hands are loaded back

// Is faster, and always advanced animation by one frame only
// In low framerates, will slow down the animation
// TODO: Insert the concept of frame period
// 1) First Render Call, 2) Updated, 4) End

void	quad_draw(t_mat32 frame, t_quad quad)
{
	int32_t	y;
	int32_t	x;
	t_vec2	dst;

	y = 0;
	while (y <= quad.size.y.i)
	{
		x = 0;
		while (x <= quad.size.x.i)
		{
			dst = (t_vec2){.x.i = quad.pos.x.i + x, .y.i = quad.pos.y.i + y};
			if (vec2_idist(quad.center, dst) <= quad.radius * quad.radius)
				pixel_swap(frame, dst.x.i, dst.y.i, quad.color);
			x++;
		}
		y++;
	}
}

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

void	pixel_swap(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
	uint32_t	*dst;

	if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
		return ;
	dst = frame.ptr + x * frame.stride + y;
	if ((*dst == 0x401013 || *dst == 0x3a0c0e)
		&& color != 0x005500)
		return ;
	if (color == 0x000000)
	{
		if (*dst == 2693401)
			color = 0x401013;
		else if (*dst == 2299157)
			color = 0x3a0c0e;
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
