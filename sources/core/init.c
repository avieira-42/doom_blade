/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 15:30:14 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 17:29:42 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cmlx.h"
#include "cub_structs.h"
#include "cub_utils.h"

int	cub_init(t_xvar *mlx, t_map *map)
{
	
	ft_memset(mlx, 0, sizeof(*mlx));
	ft_memset(map, 0, sizeof(*map));
}

void	screen_init(t_game *game)
{
	game->mlx_ptr = mlx_init();
	game->win_ptr = mlx_new_window(game->mlx_ptr, SCREEN_X, SCREEN_Y, "doom_blade");
	game->t0 = time_get();
	time_delta_get(game);
	game->vd = 1 / u_rsqrt(game->map.width * game->map.width
			+ game->map.height * game->map.height);
	game->frame.img = mlx_new_image(game->mlx_ptr, SCREEN_X, SCREEN_Y);
	game->frame.addr = mlx_get_data_addr(
			game->frame.img, &game->frame.bpp, &game->frame.l_len,
				&game->frame.endian);
	if (game->frame.addr == NULL)
		return ;
}

void	player_init(t_game *game)
{
	game->player.dir = (t_vecf32){0.71, 0.71};
	game->player.ori = (t_vecf32){0, 0};
	game->player.dir_mod = 0;
	game->player.speed = 3;
	game->player.speed_mod = 1;
	game->player.mouse_mov = (t_vecf32){0, 0};
}

void	cam_init(t_cam *cam, t_player player)
{
	cam->dist = 1;
	cam->dist_mod = 0;
	cam->half_len = 10;
	cam->dir = (t_vecf32){-0.71, 0.71};
	cam->pos = vec_sum(player.pos,
				vec_scalar_mult(player.dir, cam->dist));
}

void	game_init(t_game *game)
{
	screen_init(game);
	time_delta_get(game);
	player_init(game);
	cam_init(&game->cam, game->player);

	// DEBUG
	game->here = 0;
	game->start = 0;
}