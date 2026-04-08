/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:30:00 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/09 00:24:25 by adeimlin         ###   ########.fr       */
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

// Save max wall width and height
// Validate wall exterior
// Only one N/S/W/E
// Map content has to be last
// Dont forget to convert everything inside walls to 0 (NESW)

// Returns: >0) Ok, -1) Invalid value (P), -2) Two Player Positions (P)
static ssize_t	stt_parse_line(t_game *game, const char *line, t_map *map, t_player *player)
{
	char		c;
	const char	*oline = line;
	size_t		width;

	while (*line != '\n' && *line != 0)
	{
		c = *line;
		if (c == '0' || c == '1' || c == '2' || ft_isspace(c))
			line++;
		else if (c == 'N' || c == 'E' || c == 'S' || c == 'W')
		{
			if (player->cam.pos.x.f != 0.0f)
				return (cub_cleanup(game, "Two player positions"));
			player->cam.pos.x.f = (line++ - oline) + 0.5f;
			player->cam.pos.y.f = map->height + 0.5f;
			player->cam.dir.y.f = (c == 'N') - (c == 'S');
			player->cam.dir.x.f = (c == 'W') - (c == 'E');
		}
		else
			return (cub_cleanup(game, "Invalid map index"));
	}
	width = (size_t)(line - oline);
	if (width > map->width)
		map->width = width;
	return ((ssize_t)width + (*line == '\n'));
}

static
void	stt_filtercpy(const char *str, t_map *map)
{
	uint8_t					*dst;
	size_t					y;
	static const uint8_t	lut[256] = {
	['\t'] = 1, ['\n'] = 1, ['\v'] = 1, ['\f'] = 1, ['\r'] = 1, [' '] = 1,
	['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4, ['5'] = 5,
	['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9};

	y = 0;
	while (*str != 0)
	{
		dst = (uint8_t *) map->tex_index + y * map->width;
		while (*str != 0 && *str != '\n')
		{
			*dst = lut[(uint8_t)str[0]];
			dst++;
			str++;
		}
		str += (*str != 0);
		y++;
	}
}

static
int	stt_validate_map(t_map *map)
{
	size_t			i;
	uint8_t			*ptr1;
	uint8_t			*ptr2;
	const size_t	total_length = map->height * map->width;

	i = 0;	// Horizontal line check
	ptr1 = map->tex_index;
	ptr2 = map->tex_index + total_length - map->width;
	while (i < map->width && ptr1[i] == 1 && ptr2[i] == 1)
		i++;
	if (i != map->width)
		return (-1);	// TODO: Print error
	i = 0;	// Vertical line check
	ptr1 = map->tex_index;
	ptr2 = map->tex_index + map->width - 1;
	while (i < total_length && ptr1[i] == 1 && ptr2[i] == 1)
		i += map->width;
	if (i != total_length)
		return (-1);	// TODO: Print error
	return (0);
}

static
void	stt_init_map(t_map *map)
{
	size_t			i;
	const size_t	length = map->width * map->height;

	i = 0;
	while (i < length)
	{
		if (map->tex_index[i] > 0)
			map->tex_index[i] |= 128;	// Sets the collision bit
		i++;
	}
}

int	cub_read_map(t_game *game, const char *str, t_map *map, t_player *player)
{
	ssize_t		offset;
	const char	*ostr = str;

	while (*str != 0)
	{
		offset = stt_parse_line(game, str, map, player);
		str += offset;
		map->height++;
	}
	if (map->height < 3 || map->width < 3)
		return (cub_cleanup(game, "Invalid map configuration"));	// REVIEW
	map->tex_index = malloc(map->height * map->width * 2);
	if (map->tex_index == NULL)
		return (cub_cleanup(game, "Malloc failure"));
	map->state = map->tex_index + map->height * map->width;
	ft_memset(map->tex_index, 1, map->height * map->width);
	ft_memset(map->state, 255, map->height * map->width);
	stt_filtercpy(ostr, map);
	if (game->player.cam.dir.x.f == 0.0f && game->player.cam.dir.y.f == 0.0f)
		return (cub_cleanup(game, "Player was not found in the map"));
	if (stt_validate_map(map) == -1)
		return (cub_cleanup(game, "Invalid map configuration"));	// TODO: free and print
	stt_init_map(map);
	return (0);
}
