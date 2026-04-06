/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_texture.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:57 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/06 16:04:54 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cmlx_base.h"
#include "cub_defines.h"
#include "mlx.h"

// Assumes source is not transposed
static inline
uint32_t	stt_nearest_neighbour(const t_mat32 *src, t_vec2 norm_pos)
{
	uint32_t	x;
	uint32_t	y;

	x = roundf(norm_pos.x.f * (src->width - 1));
	y = roundf(norm_pos.y.f * (src->height - 1));
	return (src->ptr[y * src->stride + x]);
}

// Scales a texture, storing the transposed result in dst
void	ft_near_scale(t_mat32 dst, t_mat32 src)
{
	uint32_t		*ptr;
	uint32_t		x;
	uint32_t		y;
	t_vec2			norm_pos;
	const t_vec2	delta = {.x.f = 1.0 / (dst.width - 1),
			.y.f = 1.0 / (dst.height - 1)};

	x = 0;
	norm_pos.x.f = 0.0f;
	while (x < dst.width)
	{
		y = 0;
		norm_pos.y.f = 0.0f;
		ptr = dst.ptr + x * dst.stride;
		while (y < dst.height)
		{
			ptr[y] = stt_nearest_neighbour(&src, norm_pos);
			norm_pos.y.f += delta.y.f;
			y++;
		}
		norm_pos.x.f += delta.x.f;
		x++;
	}
}

static inline
void	stt_clean_texture(uint32_t *ptr, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (ptr[i] == ALPHA_SENTINEL1 || ptr[i] == ALPHA_SENTINEL2)
			ptr[i] = 0;
		i++;
	}
}

int	cub_read_texture(t_xvar *mlx, t_img *img, t_mat32 *dst)
{
	t_mat32	src;

	src = (t_mat32){(uint32_t *)img->data, img->width, img->height, 1, img->width};
	ft_near_scale(*dst, src);
	mlx_destroy_image(mlx, img);
	stt_clean_texture(dst->ptr, dst->height * dst->width);
	if (src.width != dst->width || src.height != dst->height)
		return (-1);
	return (0);
}
