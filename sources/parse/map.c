/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 11:19:47 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/02 17:08:18 by adeimlin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"
#include "mlx.h"

// Save max wall width and height
// Validate wall exterior
// Only one N/S/W/E
// Map content has to be last
// Dont forget to convert everything inside walls to 0 (NESW)

static
int	stt_horizontal_check(const char *line, const char *line_end)
{
	uint8_t	flag;

	flag = 0;
	while (line < line_end)
	{
		if (line[0] == '1' && line[1] == '0')
		{
			if (flag == 1)
				return (1);
			flag = 1;
			line += 2;
			while(*line != '1' && line < line_end)
				line++;
			if (*line != '1')
				return (1);
		}
		line++;
	}
	return (0);
}

static
int	stt_vertical_check(t_map *map)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (i < map->cols)
	{
		while (j < map->rows)
		{
			
		}
		i++;
	}
}

// Returns: >0) Ok, -1) Invalid value (P), -2) Two Player Positions (P)
static
ssize_t	stt_parse_line(const char *line, t_map *map)
{
	const char	*oline = line;
	size_t		cols;

	while (*line != '\n' && *line != 0)
	{
		if (*line == '0' || *line == '1' || ft_isspace(*line))
			line++;
		else if (*line == 'N' || *line == 'E' || *line == 'S' || *line == 'W')
		{
			if (map->player_pos.col != 0)
				return (ft_error("Error\n", "", -2));	// Two player positions
			map->player_pos.row = map->rows;
			map->player_pos.col = (line++ - oline);	// Todo: Forgot to save player direction (save as float)
		}
		else
			return (ft_error("Error\n", "", -1));	// Not NESW, space or 01
	}
	cols = (size_t)(line - oline);
	if (cols > map->cols)
		map->cols = cols;
	return ((ssize_t)cols + (*line == '\n'));
}

static
void	stt_filtercpy(const char *str, t_map *map)
{
	char	*ptr;
	size_t	row;

	row = 0;
	while (*str != 0)
	{
		ptr = (char *)map->ptr + row * map->cols;
		while (*str != 0 && *str != '\n')
			*ptr++ = (*str++ == 1) + '0';	// Normalizes values to 1 or 0, becomes a problem with doors
		str += (*str != 0);
		row++;
	}
}

int	cub_validate_map(const char *str, t_map *map)
{
	ssize_t		offset;
	const char	*ostr = str;

	while (*str != 0)
	{
		offset = stt_parse_line(str, map);
		if (offset <= 0)
			return (offset);
		map->rows++;
	}
	map->ptr = malloc(map->rows * map->cols);
	if (map->ptr == NULL)
		return (-1);
	ft_memset(map->ptr, 0, map->rows * map->cols);
	stt_filtercpy(str, map);
	return (0);
}
