/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 15:46:12 by adeimlin          #+#    #+#             */
/*   Updated: 2026/02/25 11:03:14 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"
#include "mlx.h"

// Reads a string until it finds a null terminator, space or length > 255
// Returns: 
// TODO: Create a wrapper function for xpm_to_image that removes struct malloc
static
int	stt_read_xpm(t_xvar *mlx, t_map *map, size_t index, const char **filename_ptr)
{
	int			tmp;
	const char	*filename = *filename_ptr;
	char		path[256];
	size_t		length;

	if (map->textures[index] != NULL)	// Duplicate
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
	map->textures[index] = mlx_xpm_file_to_image(mlx, path, &tmp, &tmp);
	if (map->textures[index] == NULL)
		return (-1);	// Error opening file, TODO: print
	return (0);
}

static inline
int	stt_match_texture(t_xvar *mlx, t_map *map, const char **filename_ptr)
{
	int			rvalue;
	const char	*str = *filename_ptr;

	rvalue = 0;
	if (str[0] == 'N' && str[1] == 'O')
		rvalue = stt_read_xpm(mlx, map, 0, filename_ptr);
	else if (str[0] == 'E' && str[1] == 'A')
		rvalue = stt_read_xpm(mlx, map, 1, filename_ptr);
	else if (str[0] == 'S' && str[1] == 'O')
		rvalue = stt_read_xpm(mlx, map, 2, filename_ptr);
	else if (str[0] == 'W' && str[1] == 'E')
		rvalue = stt_read_xpm(mlx, map, 3, filename_ptr);
	else if (str[0] == 'F')
		return (5);				// alpha channel is unused in parsing, can signal duplicate state
	else if (str[0] == 'C')
		return (5);
	else
		return (0);
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