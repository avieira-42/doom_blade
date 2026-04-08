/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_textures.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adeimlin <adeimlin@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/07 15:30:11 by adeimlin          #+#    #+#             */
/*   Updated: 2026/04/09 00:24:44 by adeimlin         ###   ########.fr       */
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
#include "cmlx_base.h"
#include "mlx.h"

static
void	stt_texture_init(t_block *blocks, t_memory *memory)
{
	size_t		i;
	uint32_t	*ptr;
	size_t		total_size;
	t_mat32		base;

	base = (t_mat32){NULL, TEX_SIZE, TEX_SIZE, 0, TEX_SIZE};
	i = 0;
	while (i < NUM_BLOCKS)
	{
		blocks[i].north = base;
		blocks[i].south = base;
		blocks[i].east = base;
		blocks[i].west = base;
		blocks[i].north.ptr = &memory->textures[i][0][0][0];
		blocks[i].south.ptr = &memory->textures[i][1][0][0];
		blocks[i].east.ptr = &memory->textures[i][2][0][0];
		blocks[i].west.ptr = &memory->textures[i][3][0][0];
		i++;
	}
	i = 0;
	ptr = (uint32_t *)memory->textures;
	total_size = sizeof(memory->textures) / sizeof(uint32_t);
	while (i < total_size)
		ptr[i++] = rgb_pink;
}

static int	stt_read_texture(t_game *game, t_mat32 *texture, const char *filename, const char **filen_ptr)
{
	size_t		i;
	uint32_t	color;
	size_t		tex_size;

	if (texture->depth != 0) 	// Sentinel value to see if the texture has already been initialized
		return (-1);
	while (ft_isspace(*filename))
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
		cub_read_xpm(game, texture, filename, filen_ptr);
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

static inline int	stt_match_texture(t_game *game, const char *s, t_block *blk, const char **sptr)
{
	if (s[0] == 'N' && s[1] == 'O')
		return (stt_read_texture(game, &blk[1].north, s + 2, sptr));
	else if (s[0] == 'E' && s[1] == 'A')
		return (stt_read_texture(game, &blk[1].east, s + 2, sptr));
	else if (s[0] == 'S' && s[1] == 'O')
		return (stt_read_texture(game, &blk[1].south, s + 2, sptr));
	else if (s[0] == 'W' && s[1] == 'E')
		return (stt_read_texture(game, &blk[1].west, s + 2, sptr));
	else if (s[0] == 'F')
		return (stt_read_texture(game, &blk[0].south, s + 1, sptr));
	else if (s[0] == 'C')
		return (stt_read_texture(game, &blk[0].north, s + 1, sptr));
	else if (!(s[0] == 'T' && s[1] >= '2' && s[1] < ('0' + NUM_BLOCKS)))
		return (0);
	if (s[2] == 'N')
		return (stt_read_texture(game, &blk[s[1] - '0'].north, s + 3, sptr));
	else if (s[2] == 'E')
		return (stt_read_texture(game, &blk[s[1] - '0'].east, s + 3, sptr));
	else if (s[2] == 'S')
		return (stt_read_texture(game, &blk[s[1] - '0'].south, s + 3, sptr));
	else if (s[2] == 'W')
		return (stt_read_texture(game, &blk[s[1] - '0'].west, s + 3, sptr));
	return (0);
}

int	cub_parse_textures(t_game *game, const char **str_ptr, t_memory *memory)
{
	const char	*str = *str_ptr;
	const char	*end = stt_parse_length(str, str_ptr);

	if (end == NULL)
		return (cub_cleanup(game, "Map in incorrect format"));
	stt_texture_init(game->blocks, memory);
	while (str < end)
	{
		while (ft_isspace(*str))
			str++;
		if (stt_match_texture(game, str, game->blocks, &str) < 0)
			return (cub_cleanup(game, "Duplicate textures"));
		str++;
	}
	return (0);
}
