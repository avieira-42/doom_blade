/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:30:38 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 22:09:35 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

static inline
void	stt_putrgb(t_mat32 *frame, int32_t x, int32_t y, uint32_t color)
{
	if (x < 0 || y < 0 || x >= frame->width || y >= frame->height)
		return ;
	frame->ptr[frame->stride * x + y] = color;
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
