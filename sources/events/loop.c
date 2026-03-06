/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:22:52 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 12:48:44 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

int	game_loop(void *arg)
{
	t_game *game;

	game = (t_game *)arg;
	player_move(game, &game->player, &game->cam, game->dt);
	camera_move(&game->player, &game->cam);
	mlx_mouse_move(game->mlx, game->win_ptr, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	window_clear(game, 0x000000);
	space_render(game);
	mlx_put_image_to_window(game->mlx, game->win_ptr, game->frame.img, 0, 0);
	return (1);
}
