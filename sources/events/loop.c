/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/06 12:22:52 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/09 17:47:30 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

int	cmlx_loop(t_game *game)
{
	raycast(&game->player.cam, &game->map, game->blocks, game->render_frame);
	ft_integer_scaling_t(game->render_frame, game->frame, UPSCALING_FACTOR);
	mlx_put_image_to_window(game->mlx, game->mlx->win_list, game->frame, 0, 0);
	return (1);
}
