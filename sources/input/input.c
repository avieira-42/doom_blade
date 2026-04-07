/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:29:09 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/07 16:14:48 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <X11/Xutil.h>
#include <X11/keysym.h>
#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_move(t_game *game)
{
	float	spd_forward;
	float	spd_sideway;

	spd_forward = !!(game->state.key & key_w) - !!(game->state.key & key_s);
	spd_sideway = !!(game->state.key & key_d) - !!(game->state.key & key_a);
	game->player.spd_forward = spd_forward;
	game->player.spd_sideway = spd_sideway;
	if (game->state.key & key_shift)
		game->player.spd_max = PLAYER_WALK_SPEED;
	else
		game->player.spd_max = PLAYER_RUN_SPEED;
	if (game->state.key & key_left)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, -PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, -PI_DEG);
	}
	if (game->state.key & key_right)
	{
		game->player.cam.dir = vec2_rotate(game->player.cam.dir, PI_DEG);
		game->player.cam.plane = vec2_rotate(game->player.cam.plane, PI_DEG);
	}
}

static inline
void	stt_minimap(t_game *game, t_player *player)
{
	if (game->state.key & key_tab && !(player->state & st_reloading))
		game->player.map |= st_raising;
	if (!(game->state.key & key_tab) || player->state & st_reloading)
	{
		game->player.map &= ~(size_t)st_checking;
		game->player.map &= ~(size_t)st_raising;
	}
}

void	input_handler(t_game *game, t_player *player)
{
	player->state |= st_idle;
	if (game->state.key & (key_w | key_s | key_d | key_a))
		player->state |= st_run;
	if ((game->state.key & key_rmb)
		&& !(player->state & st_shooting) && player->ammo > 0)
	{
		player->ammo--;
		player->state = (st_shooting | st_shot);
	}
	if (!(player->state & (st_reloading | st_shooting))
		&& (game->state.key & key_r) && player->ammo < AMMO_COUNT)
	{
		player->state = (st_reloading);
		player->hands.reload.index = player->ammo * RELOAD_CYCLE;
		player->map &= ~(size_t)st_raising;
		player->map &= ~(size_t)st_checking;
	}
	if (!(player->state & (st_reloading | st_shooting))
		&& (game->state.key & key_e))
	{
		player->state = st_interacting;
		game->state.key &= ~(size_t) key_e;
	}
	stt_minimap(game, player);
	stt_move(game);
}
