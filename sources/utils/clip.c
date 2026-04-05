/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clip.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/05 14:50:05 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/05 15:07:12 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"

t_sides	cub_center_clip(t_mat32 frame, t_vec2 draw_pos, t_vec2 new_size)
{
	t_sides			bounds;

	bounds.x = draw_pos.x.i - new_size.x.i / 2;
	bounds.y = draw_pos.y.i - new_size.y.i / 2;
	bounds.left = ft_iclamp(bounds.x, 0, frame.width);
	bounds.right = ft_iclamp(draw_pos.x.i + new_size.x.i / 2, 0, frame.width);
	bounds.top = ft_iclamp(bounds.y, 0, frame.height);
	bounds.bottom = ft_iclamp(draw_pos.y.i + new_size.y.i / 2, 0, frame.height);
	return (bounds);
}
