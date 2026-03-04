/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:46:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/03 15:08:58 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"
#include "mlx.h"

// Reads a string until it finds a null terminator, space or length > 255
// Returns: 
// TODO: Create a wrapper function for xpm_to_image that removes struct malloc
// TODO: Function that rescales, mallocs and frees the image after scaling
static
t_img	*stt_read_xpm(t_xvar *mlx, const char **filename_ptr)
{
	t_img		*ptr;
	int			tmp;
	const char	*filename = *filename_ptr;
	char		path[256];
	size_t		length;

	length = 0;
	while (ft_isspace(filename[length]))
		length++;
	while (filename[length] != 0 && !ft_isspace(filename[length]))
	{
		if (length >= 255)
			return (NULL);	// Path too long, TODO: print
		path[length] = filename[length];
		length++;
	}
	path[length] = 0;
	*filename_ptr = filename;
	ptr = mlx_xpm_file_to_image(mlx, path, &tmp, &tmp);
	if (ptr == NULL)
		return (NULL);	// Error opening file, TODO: print
	return (ptr);
}

static
void	stt_cleanup(void *ptr1, void *ptr2, void *ptr3)
{
	free(ptr1);
	free(ptr2);
	free(ptr3);
}

static
void	stt_transpose(t_mat *src, t_mat *dst)
{
	size_t		i;
	size_t		j;
	uint32_t	*dst_ptr;
	uint32_t	*src_ptr;

	i = 0;
	src_ptr = src->ptr;
	dst_ptr = dst->ptr;
	while (i < src->rows)
	{
		j = 0;
		while (j < src->cols)
		{
			dst_ptr[i * src->cols + j] = src_ptr[j * src->cols + i];
			j++;
		}
		i++;
	}
}

t_mat	read_texture(t_xvar *mlx, const char **filename_ptr)
{
	t_img	*img;
	t_mat	texture;
	void	*tmp;

	texture.rows = RENDER_HEIGHT;
	img = stt_read_xpm(mlx, filename_ptr);
	if (img == NULL)
		return (texture);
	texture.cols = (double)texture.rows / (double)img->height * img->width;
	texture.ptr = malloc(texture.cols * texture.rows * sizeof(uint32_t));
	if (texture.ptr == NULL)
		return (stt_cleanup(img->data, img, NULL), texture);
	// bilinear scale to A x RENDER_HEIGHT
	// Transpose to RENDER_HEIGHT x A
	texture.depth = 1;
	texture.type_size = sizeof(uint32_t);
	texture.flags = 0;
	free(img->data);
	free(img);
	return (texture);
}
