/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   new_update.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 16:39:17 by avieira-          #+#    #+#             */
/*   Updated: 2026/03/06 12:46:35 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <stdint.h>
#include <sys/time.h>
#include "cub_structs.h"

// bool	collision_check(t_game *game, t_vec2 new_pos)
// {
// 	if (game->map.grid[(int)new_pos.y][(int)new_pos.x] == '1')
// 		return (true);
// 	return (false);
// }

void		player_move(t_game *game, t_player *player, t_cam *cam, float dt)
{
	t_vec2	increment_vec_x;
	t_vec2	increment_vec_y;
	t_vec2	increment_vec;

	increment_vec_x = vec_scalar_mult(cam->dir, player->ori.x * player->speed * dt);
	increment_vec_y = vec_scalar_mult(player->dir, player->ori.y * player->speed * dt * player->speed_mod);
	increment_vec = vec_sum(increment_vec_x, increment_vec_y);
	increment_vec_x = (t_vec2){increment_vec.x, 0};
	increment_vec_y = (t_vec2){0, increment_vec.y};
	if (collision_check(game, vec_sum(increment_vec_x, player->pos)) == false)
		player->pos = vec_sum(increment_vec_x, player->pos);
	if (collision_check(game, vec_sum(increment_vec_y, player->pos)) == false)
		player->pos = vec_sum(increment_vec_y, player->pos);

	if (player->mouse_mov.x == 0)
	{
		if (player->dir_mod == -1)
			player->dir = vec_rotate(player->dir, 1, RIGHT);
		if (player->dir_mod == 1)
			player->dir = vec_rotate(player->dir, 1, LEFT);
	}
	else
	{
		if (player->mouse_mov.x > 0.)
			player->dir = vec_rotate(player->dir, player->mouse_mov.x, RIGHT);
		if (player->mouse_mov.x < 0.)
			player->dir = vec_rotate(player->dir, player->mouse_mov.x * -1, LEFT);
	}
}

void	camera_move(t_player *player, t_cam *cam)
{
	if (player->mouse_mov.x == 0)
	{
		if (player->dir_mod == -1)
			cam->dir = vec_rotate(cam->dir, 1, RIGHT);
		if (player->dir_mod == 1)
			cam->dir = vec_rotate(cam->dir, 1, LEFT);
	}
	else
	{
		if (player->mouse_mov.x > 0.)
			cam->dir = vec_rotate(cam->dir, player->mouse_mov.x, RIGHT);
		if (player->mouse_mov.x < 0.)
			cam->dir = vec_rotate(cam->dir, player->mouse_mov.x * -1, LEFT);
	}
	cam->dist += 0.1 * cam->dist_mod;
	if (cam->dist <= 1)
		cam->dist = 1;
	cam->pos = vec_sum(player->pos, vec_scalar_mult(player->dir, cam->dist));
}
