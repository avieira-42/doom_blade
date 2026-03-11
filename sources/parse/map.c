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
static
ssize_t	stt_parse_line(const char *line, t_mat8 *map, t_entity *player)
{
	const char	*oline = line;
	size_t		cols;

	while (*line != '\n' && *line != 0)
	{
		if (*line == '0' || *line == '1' || ft_isspace(*line))
			line++;
		else if (*line == 'N' || *line == 'E' || *line == 'S' || *line == 'W')
		{
			if (player->cam.pos.x.f != 0.0f)
				return (-2);	// TODO: PRINT Two player positions
			player->cam.pos.x.f = (line++ - oline) + 0.5f;	// Todo: Forgot to save player direction (save as float)
			player->cam.pos.y.f = map->rows + 0.5f;
		}
		else
			return (-1);	// Not NESW, space or 01
	}
	cols = (size_t)(line - oline);
	if (cols > map->cols)
		map->cols = cols;
	return ((ssize_t)cols + (*line == '\n'));
}

static
void	stt_filtercpy(const char *str, t_mat8 *map)
{
	uint8_t			*dst;
	size_t			row;
	static const uint8_t	lut[256] = {
		['\t'] = 1, ['\n'] = 1, ['\v'] = 1, ['\f'] = 1, ['\r'] = 1, [' '] = 1,
		['0'] = 0, ['1'] = 1};

	row = 0;
	while (*str != 0)
	{
		dst = (uint8_t *) map->ptr + row * map->cols;
		while (*str != 0 && *str != '\n')
		{
			*dst = lut[(uint8_t)*str];
			dst++;
			str++;
		}
		str += (*str != 0);
		row++;
	}
}

int	cub_read_map(const char *str, t_mat8 *map, t_entity *player)
{
	ssize_t		offset;
	const char	*ostr = str;

	while (*str != 0)
	{
		offset = stt_parse_line(str, map, player);
		if (offset <= 0)
			return (offset);
		str += offset;
		map->rows++;
	}
	map->ptr = malloc(map->rows * map->cols);
	if (map->ptr == NULL)
		return (-1);
	ft_memset(map->ptr, 1, map->rows * map->cols);
	stt_filtercpy(ostr, map);
	return (0);
}
