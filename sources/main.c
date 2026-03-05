/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 17:29:05 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 18:27:21 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "cub_structs.h"

void	update(t_game *game)
{
	time_delta_get(game);
	player_move(game, &game->player, &game->cam, game->dt);
	camera_move(&game->player, &game->cam);
	mlx_mouse_move(game->mlx, game->win_ptr, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
}

void	render(t_game *game)
{
	window_clear(game, 0x000000);
	space_render(game);
	mlx_put_image_to_window(game->mlx, game->win_ptr,
			game->frame.img, 0, 0);
}

int	game_loop(void *arg)
{
	t_game *game;

	game = (t_game *)arg;
	update(game);
	render(game);
	return (1);
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (argc != 2)
		return (1);

	if (parse(&game, argv[1]) == -1)
		return (1);
	game_init(&game);
	mlx_mouse_hide(game.mlx, game.win_ptr);
	mlx_hook(game.win_ptr, 02, (1L << 0), key_pressed, &game);
	mlx_hook(game.win_ptr, 03, (1L << 1), key_released, &game);
	mlx_hook(game.win_ptr, 04, (1L << 2), mouse_pressed, &game);
	mlx_hook(game.win_ptr, 05, (1L << 3), mouse_released, &game);
	mlx_hook(game.win_ptr, 06, (1L << 6), mouse_move, &game);
	mlx_hook(game.win_ptr, 17, 0, free_displays, &game);
	mlx_loop_hook(game.mlx, game_loop, &game);
	mlx_loop(game.mlx);
}
