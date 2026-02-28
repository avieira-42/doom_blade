/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 12:53:37 by adeimlin          #+#    #+#             */
/*   Updated: 2025/07/05 12:56:57 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	draw_line(t_img *img, t_vtx p0, t_vtx p1)
{
	const t_vtx2	delta = {ft_iabs(p1.x - p0.x), ft_iabs(p1.y - p0.y)};
	const int32_t	sx = (p0.x < p1.x) - (p0.x >= p1.x);
	const int32_t	sy = (p0.y < p1.y) - (p0.y >= p1.y);
	int32_t			err;
	int32_t			e2;

	err = delta.x - delta.y;
	while (1)
	{
		if (p0.x >= 0 && p0.y >= 0 && p0.x < WIDTH && p0.y < HEIGHT)
			putrgb(img, p0.x, p0.y, p0.color);
		if (p0.x == p1.x && p0.y == p1.y)
			break ;
		e2 = err * 2;
		if (e2 > -delta.y)
		{
			err -= delta.y;
			p0.x += sx;
		}
		if (e2 < delta.x)
		{
			err += delta.x;
			p0.y += sy;
		}
	}
}

static
void	dline_bresenham(t_img *img, uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1)
{
	const int32_t	dx = (x1 - x0);
	const int32_t	dy = (y1 - y0);
	int32_t			offset;

	offset = 2 * dy - dx;
	while (x0 < x1 || y0 < y1)
	{
		cmlx_putpixel(img, x0, y0, 0x00FFFFFF);
		y0 += (offset > 0);
		x0++;
		offset += (dy << 1) - (dx << (offset > 0));
	}
}