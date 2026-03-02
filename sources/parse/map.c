/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 11:19:47 by adeimlin          #+#    #+#             */
/*   Updated: 2026/03/02 14:35:12 by adeimlin         ###   ########.fr       */
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

// Returns: >0) Ok, -1) Invalid value (P), -2) Two Player Positions (P)
static
ssize_t	stt_parse_line(const char *line, t_map *map)
{
	const char	*oline = line;
	size_t		cols;

	cols = 0;
	while (*line != '\n' && *line != 0)
	{
		if (*line == '0' || *line == '1')
			cols++;
		else if (*line == 'N' || *line == 'E' || *line == 'S' || *line == 'W')
		{
			if (map->player_pos.col == 0)
				return (ft_error("Error\n", "", -2));	// Two player positions
			map->player_pos.row = map->rows;
			map->player_pos.col = cols++;	// Todo: Forgot to save player direction (save as float)
		}
		else if (!ft_isspace(*line))
			return (ft_error("Error\n", "", -1));	// Not NESW, space or 01
		line++;
	}
	if (cols > map->cols)
		map->cols = cols;
	return ((line - oline) + (*line == '\n'));
}

static inline const
char	*stt_max(const char *str, size_t *max, t_vec2 *player_pos)
{
	const char	*ostr = str;
	uint8_t		flag;

	flag = 0;
	while (*str != 0 && *str != '\n')
	{
		if (str[0] == '1' && str[1] == '0')
		{
			if (flag == 1)
				return (NULL);
			flag = 1;
			str += 2;
			while(*str != '1' && *str != '\n' && *str != 0)
				str++;
			if (*str != '1')
				return (NULL);
		}
		str++;
	}
	if ((size_t)(str - ostr) > *max)
		*max = (size_t)(str - ostr);
	return (str);
}

static
void	*stt_row_check(const char *str)
{
	void	*ptr;
	size_t	cols_max;
	size_t	rows;
	t_vec2	player_pos;

	rows = 0;
	cols_max = 0;
	player_pos.row = SIZE_MAX;
	while (*str != 0 && *str != '\n')
	{
		str = stt_max(str, &cols_max, &player_pos);
		str += *str != 0;
		rows++;
	}
	ptr = malloc(rows * cols_max);
	if (ptr == NULL)
		return (NULL);
	ft_memset(ptr, 0, rows * cols_max);
	return (ptr);
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
	return (0);
}
