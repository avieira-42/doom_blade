/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_state.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:29:04 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 19:39:59 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx_base.h"
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static inline
void	stt_minimap(t_player *player, t_map *map, long dt)
{
	t_sheet	*sheet;

	(void) map;
	if (!(player->state & st_radar))
		return ;
	sheet = &player->hands.radar;
	if (player->hands.radar.index < (player->hands.radar.count - 1))
		cub_advance_animation(sheet, dt);
	player->viewmodel[1] = sheet;
}

static inline
int	stt_reloading(t_player *player, t_map *map, long dt)
{
	int		rvalue;
	t_sheet	*sheet;

	(void) map;
	if (!(player->state & st_reloading))
		return (0);
	sheet = &player->hands.reload;
	rvalue = cub_advance_animation(sheet, dt);
	if (rvalue >= 2 && (sheet->index % RELOAD_CYCLE == 0))
		player->ammo++;
	if (rvalue >= 4)
	{
		player->state &= ~(size_t)st_reloading;
		return (0);	// Do not exit, a sheet was not assigned
	}
	player->viewmodel[0] = sheet;
	player->viewmodel[1] = NULL;
	return (2);	// Returns how many sheets were assigned
}

static inline
int	stt_shooting(t_player *player, t_map *map, long dt)
{
	int		rvalue;
	t_sheet	*sheet;

	(void) map;
	if (!(player->state & st_shooting))
		return (0);
	sheet = &player->hands.shoot;
	rvalue = cub_advance_animation(sheet, dt);
	if (rvalue <= 4)
	{
		player->viewmodel[0] = sheet;
		return (1);
	}
	player->state &= ~(size_t)st_shooting;
	return (0);
}

static inline
void	stt_interacting(t_player *player, t_map *map, long dt)
{
	size_t	index;

	(void) dt;
	if (!(player->state & st_interacting))
		return ;
	player->state &= ~(size_t) st_interacting;
	index = cub_first_neighbour(player->cam.pos, player->cam.dir, map->width);
	if (map->tex_index[index] == 130)
	{
		map->tex_index[index] &= 127;
		map->state[index] = 0;
	}
	else if (map->tex_index[index] == 2)
	{
		map->tex_index[index] |= 128;
		map->state[index] = 255;
	}
	return ;
}

void	update_player_state(t_player *player, t_map *map, long dt)
{
	player->viewmodel[0] = NULL;
	player->viewmodel[1] = NULL;
	stt_interacting(player, map, dt);
	if (stt_reloading(player, map, dt))
		return ;
	stt_shooting(player, map, dt);
	stt_minimap(player, map, dt);
	if (player->viewmodel[0] != NULL)
		return ;
	if (player->state & st_run)
	{
		player->viewmodel[0] = &player->hands.walk;
		cub_advance_animation(&player->hands.walk, dt);
		return ;
	}
	player->viewmodel[0] = &player->hands.idle;
	cub_advance_animation(&player->hands.idle, dt);
}
