/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_minimap.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 14:32:37 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 14:44:23 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Draws a square starting at pos a
void	quad_draw(t_vecf32 a, t_game *game, int color, t_vecf32 len)
{
	int			x;
	int			y;

	y = 0;
	while (y < len.y)
	{
		x = 0;
		while (x < len.x)
		{
			img_pixel_put(game, a.x + x, a.y + y, color);
			x++;
		}
		y++;
	}
}

void    grid_draw(t_game *game, t_vecf32 map_max, t_vecf32 map_tile, t_veci32 map_size)
{
	int	i;
	float	pos;

	i = 0;
	pos = 0;
	while (i <= map_size.y)
	{
		line_draw_bresenham((t_vecf32){0, pos}, (t_vecf32){map_max.x, pos}, game, 0x777777);
		pos += map_tile.y;
		i++;
	}
	i = 0;
	pos = 0; while (i <= map_size.x)
	{
		line_draw_bresenham((t_vecf32){pos, 0},
				(t_vecf32){pos, map_max.y},
				game, 0x777777);
		pos += map_tile.x;
		i++;
	}
}

void	objects_draw(t_game *game, t_veci32 map_size, t_vecf32 map_tile)
{
	int32_t	y;
	int32_t	x;

	y = 0;
	while (y < map_size.y)
	{
		x = 0;
		while(x < map_size.x)
		{
			if (game->map.grid[y][x] == '1')
				quad_draw((t_vecf32){x * map_tile.x,
						y * map_tile.y},
						game, 0x555555, map_tile);
			x++;
		}
		y++;
	}
}

void	player_draw(t_game *game, t_vecf32 map_tile)
{
	t_vecf32	p_pos;

	p_pos.x = (game->player.pos.x - 0.5f) * map_tile.x;
	p_pos.y = (game->player.pos.y - 0.5f) * map_tile.y;
	quad_draw(p_pos, game, 0xFF00FF, map_tile);
	stt_putrgb(game->frame, p_pos.x, p_pos.y, 0xFFFFFF);
}