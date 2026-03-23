/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_scaling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 15:13:28 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/23 18:19:57 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

// TODO: Assumption used to be scale to fit, review algorithm accordingly and add pos draw
void	ft_bilinear_scaling(const t_mat32 *src, t_mat32 *dst, t_vec2 scale, t_vec2 draw_pos)
{
	t_form		form;
	t_vec2		new_size;
	int32_t		unclipped_left;
	int32_t		unclipped_top;
	uint32_t	x;
	uint32_t	y;
	t_vec2		norm_pos;

	new_size.x.i = src->width * scale.x.f;
	new_size.y.i = src->height * scale.y.f;
	form.delta.x.f = 1.0 / new_size.x.i;
	form.delta.y.f = 1.0 / new_size.y.i;
	unclipped_left = (int)draw_pos.x.i;
	unclipped_top = (int)draw_pos.y.i;
	form.left = ft_iclamp(unclipped_left, 0, dst->width);
	form.top = ft_iclamp(unclipped_top, 0, dst->height);
	form.right = ft_iclamp((int)draw_pos.x.i + (int)new_size.x.i, 0, dst->width);
	form.bottom = ft_iclamp((int)draw_pos.y.i + (int)new_size.y.i, 0, dst->height);
	form.norm_offset.x.f = ((int)form.left - unclipped_left) * form.delta.x.f;		// Clipped start
	form.norm_offset.y.f = ((int)form.top - unclipped_top) * form.delta.y.f;		// Clipped start
	x = form.left;
	norm_pos.x.f = form.norm_offset.x.f;
	while (x < form.right)
	{
		y = form.top;
		norm_pos.y.f = form.norm_offset.y.f;
		while (y < form.bottom)
		{
			dst->ptr[y * dst->stride + x] = ft_bilerp_argb(src, norm_pos);	// Bilerp takes a normalized range to sample from
			norm_pos.y.f += form.delta.y.f;
			y++;
		}
		norm_pos.x.f += form.delta.x.f;
		x++;
	}
}
