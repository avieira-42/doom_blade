/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 15:32:44 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/04 13:30:00 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <math.h>
#include "fdf_utils.h"
#include "fdf.h"

static inline
void	putrgb(t_img *img, uint32_t x, uint32_t y, uint32_t argb)
{
	((uint32_t (*)[img->width])img->data)[y][x] = argb;
}

static inline
uint32_t clerp(uint32_t dst, uint32_t src, uint8_t alpha)
{
	uint32_t rb;
	uint32_t ga;

	rb = dst & 0x00FF00FF;
	ga = (dst >> 8) & 0x00FF00FF;
	rb += (((src & 0x00FF00FF) - rb) * alpha) >> 8;
	ga += ((((src >> 8) & 0x00FF00FF) - ga) * alpha) >> 8;
	return (rb & 0x00FF00FF) | ((ga & 0x00FF00FF) << 8);
}

static
void	draw_line(t_img *img, t_vtx p0, t_vtx p1)
{
	const uint32_t	len = ft_iabsmax(p1.x - p0.x, p1.y - p0.y);
	const t_delta	delta = {(double)(p1.x - p0.x) / (double)(len + 1),
		(double)(p1.y - p0.y) / (double)(len + 1), (255u << 16) / (len + 1)};
	uint32_t		alpha;
	t_vec2			vec;
	uint32_t		i;

	vec.x = p0.x;
	vec.y = p0.y;
	i = 0;
	alpha = 0;
	while (i <= len)
	{
		if (vec.x >= 0.0f && vec.y >= 0.0f && vec.x < WIDTH && vec.y < HEIGHT)
			putrgb(img, vec.x, vec.y, clerp(p0.color, p1.color, alpha >> 16));
		alpha += delta.color;
		vec.x += delta.x;
		vec.y += delta.y;
		i++;
	}
}

void	draw_neighbours(t_vars *vars, size_t row, size_t col)
{
	t_vtx			p0;
	t_vtx			p1;
	const float		s[2] = {0.5f * WIDTH * vars->params.zoom,
		0.5f * HEIGHT * vars->params.zoom};
	const uintptr_t	offset = row * vars->cols + col;
	const uintptr_t	index[2] = {offset + 1, offset + vars->cols};

	p0.x = vars->vec[offset].x * s[0] + WIDTH * 0.5f;
	p0.y = vars->vec[offset].y * s[1] + HEIGHT * 0.5f;
	p0.color = vars->vtx[offset].color;
	if (col + 1 < vars->cols)
	{
		p1.x = vars->vec[index[0]].x * s[0] + WIDTH * 0.5f;
		p1.y = vars->vec[index[0]].y * s[1] + HEIGHT * 0.5f;
		p1.color = vars->vtx[index[0]].color;
		draw_line(vars->img, p0, p1);
	}
	if (row + 1 < vars->rows)
	{
		p1.x = vars->vec[index[1]].x * s[0] + WIDTH * 0.5f;
		p1.y = vars->vec[index[1]].y * s[1] + HEIGHT * 0.5f;
		p1.color = vars->vtx[index[1]].color;
		draw_line(vars->img, p0, p1);
	}
}
