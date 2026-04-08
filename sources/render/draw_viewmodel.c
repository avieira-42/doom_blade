/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_viewmodel.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:30:42 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/08 14:17:04 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

// the result of integer division might make it unrenderable
static
void	stt_grid_draw(t_mat32 frame, t_map *map, t_vec2 grid_pos, t_quad cell)
{
	size_t		x;
	size_t		y;
	uint32_t	block;

	y = 0;
	while (y < map->height)
	{
		x = 0;
		while (x < map->width)
		{
			block = map->tex_index[x + y * map->width];
			if (block == 129)
			{
				cell.pos = (t_vec2){.x.i = grid_pos.x.i + cell.size.x.i * x,
					.y.i = grid_pos.y.i + cell.size.y.i * y};
				draw_circle(frame, cell);
			}
			x++;
		}
		y++;
	}
}

static
void	stt_player_icon_draw(t_mat32 frame, t_map *map, t_vec2 grid_pos, t_player *player)
{
	t_vec2	line_dst;
	t_quad	icon;

	icon = map->icon_quad;
	icon.pos.x.i = map->radar_sprite_pos.x.i + icon.pos.x.i - grid_pos.x.i - icon.size.x.i / 2;
	icon.pos.y.i = map->radar_sprite_pos.y.i + icon.pos.y.i	- grid_pos.y.i - icon.size.y.i / 2;
	icon.center.x.i = icon.pos.x.i + icon.radius;
	icon.center.y.i = icon.pos.y.i + icon.radius;
	icon.color = RADAR_PLAYER_COLOR;
	line_dst.x.i = icon.center.x.i + (int32_t)(player->cam.dir.x.f * 8.0f);
	line_dst.y.i = icon.center.y.i + (int32_t)(player->cam.dir.y.f * 8.0f);
	draw_line(frame, icon.center, line_dst, RADAR_FOV_COLOR);
	draw_circle(frame, icon);
}

static
void	stt_enemies_icon_draw(t_mat32 frame, t_map *map, t_vec2 grid_pos, t_enemy *enemies)
{
	int32_t	i;
	t_quad	icon;

	i = 0;
	icon = map->icon_quad;
	icon.color = RADAR_ENEMY_COLOR;
	while (i < NUM_ENEMIES)
	{
		if (enemies[i].state & e_dead)
		{
			i++;
			continue;
		}
		icon.pos = (t_vec2){.x.i = grid_pos.x.i
			+ enemies[i].cam.pos.x.f * map->radar_quad.size.x.i,
				.y.i = grid_pos.y.i + enemies[i].cam.pos.y.f * map->radar_quad.size.y.i};
		icon.center = (t_vec2){.x.i = icon.pos.x.i + icon.radius,
			.y.i = icon.pos.y.i + icon.radius};
		if (vec2_idist(map->radar_quad.center, icon.pos) < map->radar_quad.radius * map->radar_quad.radius)
			draw_circle(frame, icon);
		i++;
	}
}

static
void	stt_draw_radar(t_game *game, t_map *map)
{
	t_vec2	grid_offset;
	t_vec2	g_pos;

	map->icon_quad.pos = (t_vec2){.x.i = game->player.cam.pos.x.f * map->radar_quad.size.x.i,
			.y.i = game->player.cam.pos.y.f * map->radar_quad.size.y.i};
	grid_offset = (t_vec2){.x.i = map->icon_quad.pos.x.i - game->player.hands.radar_l0.texture.width / 2,
			.y.i = map->icon_quad.pos.y.i - game->player.hands.radar_l0.texture.height / 2};
	grid_offset.x.i = ft_imax(grid_offset.x.i, 0);
	grid_offset.x.i = ft_imin(grid_offset.x.i, map->radar_size.x.i - game->player.hands.radar_l0.texture.width);
	grid_offset.y.i = ft_imax(grid_offset.y.i, 0);
	grid_offset.y.i = ft_imin(grid_offset.y.i, map->radar_size.y.i - game->player.hands.radar_l0.texture.height);
	g_pos = (t_vec2){.x.i = map->radar_sprite_pos.x.i - grid_offset.x.i, .y.i = map->radar_sprite_pos.y.i - grid_offset.y.i};
	cub_draw_sheet(game->frame.render, &game->player.hands.radar_l0, map->radar_sprite_pos.x.i, map->radar_sprite_pos.y.i);
	stt_grid_draw(game->frame.render, map, g_pos, game->map.radar_quad);
	stt_enemies_icon_draw(game->frame.render, map, g_pos, game->enemies);
	stt_player_icon_draw(game->frame.render, map, grid_offset, &game->player);
	cub_draw_sheet(game->frame.render, &game->player.hands.radar_l1, map->radar_sprite_pos.x.i, map->radar_sprite_pos.y.i);
}

void	cub_draw_viewmodel(t_mat32 frame, t_player *player, t_game *game, long dt)
{
	size_t	x;
	size_t	y;

	(void) dt;
	x = ((r_width - player->viewmodel[0]->texture.width) / 2) + 5;
	y = r_height - player->viewmodel[0]->texture.height;
	cub_draw_sheet(frame, player->viewmodel[0], x, y);
	if (player->viewmodel[1] != NULL)
	{
		cub_draw_sheet(frame, player->viewmodel[1], 0, 200);
		if (player->hands.radar.index == (player->hands.radar.count - 1))
			stt_draw_radar(game, &game->map);
	}
}
