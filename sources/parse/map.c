/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/06 11:19:47 by adeimlin          #+#    #+#             */
/*   Updated: 2026/02/18 16:30:18 by adeimlin         ###   ########.fr       */
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
			map->player_pos.col = cols++;	// Todo: Forgot to save player direction
		}
		else if (!ft_isspace(*line))
			return (ft_error("Error\n", "", -1));	// Not NESW, space or 01
		line++;
	}
	if (cols > map->cols)
		map->cols = cols;
	return ((line - oline) + (*line == '\n'));
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

