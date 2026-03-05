/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:46:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/05 16:29:06 by adeimlin         ###   ########.fr       */
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
#include "cub_functions.h"
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

// Reads
static
int	stt_read_texture(t_xvar *mlx, t_mat32 *texture, const char **filename_ptr)
{
	t_img	*img;

	if (texture->ptr != NULL)
		return (-2);
	img = stt_read_xpm(mlx, filename_ptr);
	if (img == NULL)
		return (-1);
	texture->rows = RENDER_HEIGHT;
	texture->cols = (double)texture->rows / (double)img->height * img->width;
	texture->depth = 1;
	texture->ptr = malloc(texture->cols * texture->rows * sizeof(uint32_t));
	if (texture->ptr == NULL)
		return (mlx_destroy_image(mlx, img), -1);
	ft_bilinear_scaling(&(t_mat32){(uint32_t*)img->data, img->height, img->width, 1, 0}, texture);
	ft_transpose(texture, img->data);
	mlx_destroy_image(mlx, img);
	return (0);
}

static
int	stt_read_color(t_xvar *mlx, t_mat32 *texture, const char **filename_ptr)
{
	size_t		i;
	const char	*filename = *filename_ptr;
	uint32_t	color;

	if (texture->ptr != NULL)
		return (-1);	// TODO: Print error
	while (ft_isspace(*filename))
		filename++;
	if (filename[0] == '.' && filename[1] == '/')
		return (stt_read_texture(mlx, texture, filename_ptr));
	color = ft_strtoargb(filename, filename_ptr);
	texture->ptr = malloc(RENDER_HEIGHT * sizeof(uint32_t));		// Creates a column of colors
	if (texture->ptr == NULL)
		return (-1);	// TODO: Print error
	texture->rows = RENDER_HEIGHT;	// Consider it transposed
	texture->cols = 1;
	i = 0;
	while (i < RENDER_HEIGHT)
		texture->ptr[i++] = color;
	return (0);
}

static inline
int	stt_match_texture(t_xvar *mlx, const char *str, t_block *blocks, const char **str_ptr)
{
	int	rvalue;

	rvalue = 1;
	if (str[0] == 'N' && str[1] == 'O')
		rvalue = stt_read_texture(mlx, &blocks[0].north, str_ptr);
	else if (str[0] == 'E' && str[1] == 'A')
		rvalue = stt_read_texture(mlx, &blocks[0].east, str_ptr);
	else if (str[0] == 'S' && str[1] == 'O')
		rvalue = stt_read_texture(mlx, &blocks[0].south, str_ptr);
	else if (str[0] == 'W' && str[1] == 'E')
		rvalue = stt_read_texture(mlx, &blocks[0].west, str_ptr);
	else if (str[0] == 'F')
		rvalue = stt_read_color(mlx, &blocks[1].south, str_ptr);
	else if (str[0] == 'C')
		rvalue = stt_read_color(mlx, &blocks[1].north, str_ptr);
		// Continue here for more textures
	return (rvalue);
}

int	cub_read_textures(t_xvar *mlx, const char *str, t_block *blocks)
{
	int		rvalue;
	size_t	match_target;

	match_target = 6;	// If DOOR is found, then match_target needs to be increased
	while (*str != 0 && match_target > 0)
	{
		rvalue = stt_match_texture(mlx, str, blocks, &str);
		if (rvalue < 0)
			return (-1);	// Future exit cleanup
		else if (rvalue == 0)
			match_target--;
		else
			str++;
	}
	if (match_target != 0)
		return (-1);
	return (0);
}
