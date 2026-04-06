/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_xpm.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/06 14:20:57 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/06 18:11:19 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "cmlx_base.h"
#include "cub_utils.h"
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

static
t_img	*stt_read_xpm(t_xvar *mlx, const char *filename, const char **filename_ptr)
{
	int			tmp[2];
	char		buffer[256];
	char		*path;
	const char	*path_end = buffer + sizeof(buffer) - 1;

	path = buffer;
	while (ft_isspace(*filename))
		filename++;
	while (*filename != 0 && !ft_isspace(*filename))
	{
		if (path >= path_end)
			return (NULL);	// Path too long, TODO: print
		*path++ = *filename++;
	}
	*path = 0;
	if (filename_ptr != NULL)
		*filename_ptr = filename;
	return (mlx_xpm_file_to_image(mlx, buffer, tmp, tmp + 1));
}

// Todo: guarantee lower than render res
int	cub_read_xpm(t_xvar *mlx, t_mat32 *dst, const char *filename, const char **filename_ptr)
{
	t_img	*img;
	t_mat32	src;
	size_t	size;

	img = stt_read_xpm(mlx, filename, filename_ptr);
	if (img == NULL)
		return (-1);
	if (dst->ptr == NULL)	// DST needs to have depth defined
	{
		size = (size_t)(img->height * img->width) * sizeof(uint32_t) * dst->depth;
		*dst = (t_mat32){malloc(size), img->width, img->height, dst->depth, img->height};
		if (dst->ptr == NULL)
			return (mlx_destroy_image(mlx, img), -1);	// Malloc failure
	}
	src = (t_mat32){(uint32_t *)img->data, img->width, img->height, 1, img->width};
	ft_near_scale(*dst, src);
	mlx_destroy_image(mlx, img);
	stt_clean_texture(dst->ptr, dst->height * dst->width);
	if (src.width != dst->width || src.height != dst->height)
		return (-1);
	return (0);
}
