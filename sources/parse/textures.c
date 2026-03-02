/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:46:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/02 17:17:47 by adeimlin         ###   ########.fr       */
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
int	stt_read_xpm(t_xvar *mlx, t_mat *texture, const char **filename_ptr)
{
	int			tmp;
	const char	*filename = *filename_ptr;
	char		path[256];
	size_t		length;

	if (texture->ptr != NULL)	// Duplicate
		return (-1);
	length = 0;
	while (ft_isspace(filename[length]))
		length++;
	while (filename[length] != 0 && !ft_isspace(filename[length]))
	{
		if (length >= 255)
			return (-1);	// Path too long, TODO: print
		path[length] = filename[length];
		length++;
	}
	path[length] = 0;
	*filename_ptr = filename;
	texture->ptr = mlx_xpm_file_to_image(mlx, path, &tmp, &tmp);
	if (texture->ptr == NULL)
		return (-1);	// Error opening file, TODO: print
	return (0);
}

static inline
int	stt_read_color(t_xvar *mlx, t_mat *texture, const char **filename_ptr)
{
	size_t		i;
	const char	*filename = *filename_ptr;
	uint32_t	color;
	uint32_t	*ptr;

	while (ft_isspace(*filename))
		filename++;
	if (filename[0] == '.' && filename[1] == '/')
		return (stt_read_xpm(mlx, texture, filename_ptr));
	color = ft_strtoargb(filename, filename_ptr);
	ptr = malloc(RENDER_HEIGHT * sizeof(uint32_t));				// Creates a column of colors
	if (ptr == NULL)
		return (-1);	// TODO: Print error
	texture->ptr = (void *) ptr;
	texture->cols = RENDER_HEIGHT;
	texture->rows = 1;
	texture->line_size = RENDER_HEIGHT * sizeof(uint32_t);
	texture->type_size = sizeof(uint32_t);
	i = 0;
	while (i < RENDER_HEIGHT)
		ptr[i++] = color;
	return (0);
}

static inline
int	stt_match_texture(t_xvar *mlx, t_map *map, const char **filename_ptr)
{
	int			rvalue;
	const char	*str = *filename_ptr;

	rvalue = 0;
	if (str[0] == 'N' && str[1] == 'O')
		rvalue = stt_read_xpm(mlx, map->textures + 0, filename_ptr);
	else if (str[0] == 'E' && str[1] == 'A')
		rvalue = stt_read_xpm(mlx, map->textures + 1, filename_ptr);
	else if (str[0] == 'S' && str[1] == 'O')
		rvalue = stt_read_xpm(mlx, map->textures + 2, filename_ptr);
	else if (str[0] == 'W' && str[1] == 'E')
		rvalue = stt_read_xpm(mlx, map->textures + 3, filename_ptr);
	else if (str[0] == 'F')
		rvalue = stt_read_color(mlx, map->textures + 4, filename_ptr);	// alpha channel is unused in parsing, can signal duplicate state
	else if (str[0] == 'C')
		rvalue = stt_read_color(mlx, map->textures + 5, filename_ptr);	// alpha channel is unused in parsing, can signal duplicate state
	return (rvalue);
}

int	cub_read(const char *str, t_map *map)
{
	size_t	i;
	uint8_t	matches[6];
	int		result;

	i = 0;
	while (str[i] != 0)
	{
		result = stt_match_texture(input + i);
		if (result != 0)
		{
			
		}
	}
}