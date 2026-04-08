/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_state.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:28:49 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/08 15:07:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

static
bool	stt_enemy_dead(t_map *map, t_enemy *enemy, long dt)
{
	if (enemy->state & e_dead)
	{
		enemy->respawn_timer += dt;
		if (enemy->respawn_timer >= RESPAWN_TIMER)
		{
			enemy->health = 100;
			enemy->respawn_timer = 0;
			enemy->state = e_running;
			enemy->cam.pos = random_valid_pos(map);
		}
		return (true);
	}
	return (false);
}

static
void	stt_enemy_shooting(t_player *player, t_enemy *enemy, long dt)
{
	if (enemy->state & e_shooting)
	{
		if (cub_advance_animation(&enemy->shooting, dt) >= 4)
		{
			enemy->state &= ~(size_t)e_shooting;
			enemy->state |= e_running;
			enemy->shooting.index = 0;
			enemy->shooting.frame_dt = 0;
		}
		return ;
	}
	if ((fabsf(enemy->dist) > ENEMY_ATTACK_DIST) && (enemy->state & e_seen))
		return ;
	enemy->state &= ~(size_t)e_running;
	enemy->state |= e_shooting;
	enemy->shooting.index = 0;
	enemy->shooting.frame_dt = 0;
	if (ft_randf() < ENEMY_ACCURACY)
		player->health -= ENEMY_DAMAGE;
	enemy->state &= ~(size_t) e_seen;
}

static
void	stt_enemy_hurt(t_player *player, t_enemy *enemy, long dt)
{
	if ((player->state & st_shot) && (enemy->state & e_hit))
	{
		enemy->health -= PLAYER_DAMAGE;
		if (enemy->health <= 0)
			enemy->state = e_dying;
		else
			enemy->state = e_hurt;
	}
	if (enemy->state & e_dying)
	{
		if (cub_advance_animation(&enemy->dying, dt) >= 4)
		{
			player->health += HEAL_VALUE;
			enemy->state = e_dead;
		}
	}
	else if (enemy->state & e_hurt)
	{
		if (cub_advance_animation(&enemy->shot, dt) >= 4)
		{
			enemy->state &= ~(size_t) e_hurt;
			enemy->state |= e_running;
		}
	}
}

// Every time draw_enemy gets called, enemy state updates e_hit
void	update_enemy_state\
(t_map *map, t_player *player, t_enemy *enemy, long dt)
{
	t_sheet	*sheet;

	if (stt_enemy_dead(map, enemy, dt))
		return ;
	stt_enemy_hurt(player, enemy, dt);
	stt_enemy_shooting(player, enemy, dt);
	if (enemy->state & e_dying)
		sheet = &enemy->dying;
	else if (enemy->state & e_hurt)
		sheet = &enemy->shot;
	else if (enemy->state & e_shooting)
		sheet = &enemy->shooting;
	else
		sheet = &enemy->running;
	enemy->model = sheet;
}
