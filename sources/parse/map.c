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
				return (ft_error("Error\n", "", -2));	// Two player positions
			player->cam.pos.x.f = (line++ - oline) + 0.5f;	// Todo: Forgot to save player direction (save as float)
			player->cam.pos.y.f = map->rows + 0.5f;
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
void	stt_filtercpy(const char *str, t_mat8 *map)
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

int	cub_read_map(const char *str, t_mat8 *map, t_entity *player)
{
	ssize_t		offset;
	const char	*ostr = str;

	while (*str != 0)
	{
		offset = stt_parse_line(str, map, player);
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
