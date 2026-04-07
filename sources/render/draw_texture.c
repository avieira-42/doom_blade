/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:30:34 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 16:22:29 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_draw_texture\
(t_mat32 frame, t_mat32 image, uint32_t xc, uint32_t yc)
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
			if (color != 0)												// TODO: Guarantee function safety
				frame.ptr[(x + xc) * frame.stride + y + yc] = color;	// Review: removed putpixel because bounds can be precalcualted
			x++;
		}
		y++;
	}
}

void	cub_draw_sheet\
(t_mat32 frame, const t_sheet *sheet, uint32_t xc, uint32_t yc)
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
			if (color != 0)												// TODO: Guarantee function safety
				frame.ptr[(x + xc) * frame.stride + y + yc] = color;	// Review: removed putpixel because bounds can be precalcualted
			x++;
		}
		y++;
	}
}
