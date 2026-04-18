#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx_base.h"

static
int	stt_dispatch(t_game *game, t_mat32 *texture, const char *filename, const char **filen_ptr)
{
	size_t		i;
	uint32_t	color;
	size_t		tex_size;

	if (texture->depth != 0) 	// Sentinel value to see if the texture has already been initialized
		return (-1);
	while (isspace(*filename))
		filename++;
	i = 0;
	while (filename[i] >= '0' && filename[i] <= '9')
		i++;
	if (filename[i] == ',')
	{
		color = ft_strtoargb(filename, filen_ptr);
		tex_size = texture->width * texture->height;
		i = 0;
		while (i < tex_size)
			texture->ptr[i++] = color;
	}
	else
		cub_read_texture(game, texture, filename, filen_ptr);
	texture->depth = 1;
	return (0);
}

static
const char	*stt_parse_length(const char *str, const char **str_ptr)
{
	const char	*parse_pos = str;
	const char	*map_pos = str;

	while (*str != 0)
	{
		if (*str == ',' || *str == '.')
			parse_pos = str;
		str++;
	}
	while (*parse_pos != 0 && *parse_pos != '\n')
		parse_pos++;
	str = parse_pos;
	while (*str != 0 && *str != '1')	// Skips until the map portion
	{
		if (*str == '\n')
			map_pos = str + 1;
		str++;
	}
	*str_ptr = map_pos;
	if (*str == 0)
		return (NULL);
	return (parse_pos);
}

static inline
int	stt_match_texture(t_game *game, const char *str, t_block *block, const char **sptr)
{
	if (str[0] == 'N' && str[1] == 'O')
		return (stt_dispatch(game, &block[1].north, str + 2, sptr));
	if (str[0] == 'E' && str[1] == 'A')
		return (stt_dispatch(game, &block[1].east, str + 2, sptr));
	if (str[0] == 'S' && str[1] == 'O')
		return (stt_dispatch(game, &block[1].south, str + 2, sptr));
	if (str[0] == 'W' && str[1] == 'E')
		return (stt_dispatch(game, &block[1].west, str + 2, sptr));
	if (str[0] == 'F')
		return (stt_dispatch(game, &block[0].south, str + 1, sptr));
	if (str[0] == 'C')
		return (stt_dispatch(game, &block[0].north, str + 1, sptr));
	if (!(str[0] == 'T' && str[1] >= '2' && str[1] < ('0' + NUM_BLOCKS)))
		return (0);
	if (str[2] == 'N')
		return (stt_dispatch(game, &block[str[1] - '0'].north, str + 3, sptr));
	if (str[2] == 'E')
		return (stt_dispatch(game, &block[str[1] - '0'].east, str + 3, sptr));
	if (str[2] == 'S')
		return (stt_dispatch(game, &block[str[1] - '0'].south, str + 3, sptr));
	if (str[2] == 'W')
		return (stt_dispatch(game, &block[str[1] - '0'].west, str + 3, sptr));
	return (0);
}

int	cub_parse_textures(t_game *game, const char **str_ptr)
{
	const char	*str = *str_ptr;
	const char	*end = stt_parse_length(str, str_ptr);

	if (end == NULL)
		return (cub_cleanup(game, "Map in incorrect format"));
	while (str < end)
	{
		while (isspace(*str))
			str++;
		if (stt_match_texture(game, str, game->blocks, &str) < 0)
			return (cub_cleanup(game, "Duplicate textures"));
		str++;
	}
	return (0);
}
