/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:30:14 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 13:08:28 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cub_functions.h"

static
int	stt_mlx_init(t_game *game)
{
	game->mlx = mlx_init();
	if (game->mlx == NULL)
		return (-1);	// TODO: Print Error
	game->mlx->win_list = mlx_new_window(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "doom_blade");
	if (game->mlx->win_list == NULL)
		return (-1);
	game->frame = mlx_int_new_image(game->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, ZPixmap);
	if (game->frame == NULL)
		return (-1);
	return (0);
}

static
int	stt_mlx_hooks(t_win_list *window, t_game *game)
{
	mlx_hook(window, KeyPress, KeyPressMask, cmlx_keydown, game);
	mlx_hook(window, KeyRelease, KeyReleaseMask, cmlx_keyup, game);
	mlx_hook(window, ButtonPress, ButtonPressMask, cmlx_mousedown, game);
	mlx_hook(window, ButtonRelease, ButtonReleaseMask, cmlx_mouseup, game);
	mlx_hook(window, MotionNotify, PointerMotionMask, cmlx_mousemove, game);
	mlx_mouse_hide(game->mlx, window);
	// mlx_loop_hook(game->mlx, cmlx_loop, game);	//
	return (0);
}

void	game_init(t_game *game)
{
	ft_memset(game, 0, sizeof(*game));
	game->vd = ft_qsqrt(game->map.cols * game->map.cols + game->map.rows * game->map.rows);
	game->player.dir = (t_vec2){.x.f = 0.71f, .y.f = 0.71f};
	game->player.ori = (t_vec2){.x.f = 0.0f, .y.f = 0.0f};
	game->player.dir_mod = 0;
	game->player.speed = 3;
	game->player.speed_mod = 1;
	game->cam.mouse_mov = (t_vec2){.x.f = 0.0f, .y.f = 0.0f};
	game->cam.dist = 1;
	game->cam.dist_mod = 0;
	game->cam.half_len = 10;
	game->cam.dir = (t_vec2){.x.f = 0.71f, .y.f = 0.71f};
	game->cam.pos = vec_sum(player.pos, vec_scalar_mult(player.dir, cam->dist));

	// DEBUG
	game->here = 0;
	game->start = 0;
}