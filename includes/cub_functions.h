/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub_functions.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/05 13:46:57 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/06 13:00:35 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB_FUNCTIONS_H
# define CUB_FUNCTIONS_H

#include "cub_structs.h"

void	ft_bilinear_scaling(t_mat32 *src, t_mat32 *dst);
int		ft_transpose(t_mat32 *src, void *dst);

int	cmlx_keydown(int keycode, t_game *game);
int	cmlx_keyup(int keycode, t_game *game);
int	cmlx_mousedown(int button, int32_t x, int32_t y, t_game *game);
int	cmlx_mouseup(int button, int32_t x, int32_t y, t_game *game);
int	cmlx_mousemove(int32_t x1, int32_t y1, t_game *game);

#endif