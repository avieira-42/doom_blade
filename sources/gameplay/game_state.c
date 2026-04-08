/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:28:54 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/08 15:08:07 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"

void	cub_update_pos(t_game *game, float dt);
void	update_enemy_state(t_map *map, t_player *p, t_enemy *enemy, long dt);
void	update_player_state(t_player *player, t_map *map, long dt);

void	cub_update_game(t_game *game, long dt)
{
	size_t	i;
	t_enemy	*enemy;

	cub_update_pos(game, (double)dt * 0.000001);
	update_player_state(&game->player, &game->map, dt);
	i = 0;
	while (i < NUM_ENEMIES)
	{
		enemy = game->enemies + i;
		update_enemy_state(&game->map, &game->player, enemy, dt);
		i++;
	}
}
