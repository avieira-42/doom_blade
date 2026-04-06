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

// Reads a string until it finds a null terminator, space or length > 255
// Returns: 
static
t_img	*stt_read_xpm(t_xvar *mlx, const char *filename, const char **filename_ptr)
{
	t_img		*ptr;
	int			tmp[2];
	char		buffer[256];
	char		*path;
	const char	*path_end = buffer + sizeof(buffer) - 1;

	path = buffer;
	while (ft_isspace(*filename))
		filename++;
	while (*filename != 0 && !ft_isspace(*filename))
	{
		if (path >= path_end)
			return (NULL);	// Path too long, TODO: print
		*path++ = *filename++;
	}
	*path = 0;
	if (filename_ptr != NULL)
		*filename_ptr = filename;
	ptr = mlx_xpm_file_to_image(mlx, buffer, tmp, tmp + 1);
	if (ptr == NULL)
		return (NULL);	// Error opening file, TODO: print
	return (ptr);
}

// Reads an xpm texture, scales it to TEX_SIZE, transposes the result
// and frees the original MLX img ptr
static
int	stt_read_texture(t_xvar *mlx, t_mat32 *texture, const char *filename, const char **filename_ptr)
{
	t_img	*img;

	if (texture->depth != 0) 	// Sentinel value to see if the texture has already been initialized
		return (-2);
	img = stt_read_xpm(mlx, filename, filename_ptr);
	if (img == NULL)
		return (-1);
	cub_read_texture(mlx, img, texture);
	texture->depth = 1;	// Sets the sentinel value
	return (0);
}

static
int	stt_read_color(t_xvar *mlx, t_mat32 *texture, const char *filename, const char **filename_ptr)
{
	size_t		i;
	uint32_t	color;
	size_t		tex_size;

	if (texture->depth != 0) 	// Sentinel value to see if the texture has already been initialized
		return (-1);	// TODO: Print error
	while (ft_isspace(*filename))
		filename++;
	i = 0;
	while (filename[i] >= '0' && filename[i] <= '9')
		i++;
	if (filename[i] != ',')
		return (stt_read_texture(mlx, texture, filename, filename_ptr));
	color = ft_strtoargb(filename, filename_ptr);
	tex_size = texture->width * texture->height;
	i = 0;
	while (i < tex_size)
		texture->ptr[i++] = color;
	texture->depth = 1;
	return (0);
}

static inline
int	stt_match_texture(t_xvar *mlx, const char *str, t_block *blocks, const char **str_ptr)
{
	int	rvalue;

	while (ft_isspace(*str))
		str++;
	rvalue = 1;
	if (str[0] == 'N' && str[1] == 'O')
		rvalue = stt_read_texture(mlx, &blocks[1].north, str + 2, str_ptr);
	else if (str[0] == 'E' && str[1] == 'A')
		rvalue = stt_read_texture(mlx, &blocks[1].east, str + 2, str_ptr);
	else if (str[0] == 'S' && str[1] == 'O')
		rvalue = stt_read_texture(mlx, &blocks[1].south, str + 2, str_ptr);
	else if (str[0] == 'W' && str[1] == 'E')
		rvalue = stt_read_texture(mlx, &blocks[1].west, str + 2,  str_ptr);
	else if (str[0] == 'F')
		rvalue = stt_read_color(mlx, &blocks[0].south, str + 1, str_ptr);
	else if (str[0] == 'C')
		rvalue = stt_read_color(mlx, &blocks[0].north, str + 1, str_ptr);
	else if (str[0] == 'D')
	{
		rvalue = stt_read_texture(mlx, &blocks[2].north, str + 1, str_ptr);
		blocks[2].south = blocks[2].north;
		blocks[2].west = blocks[2].north;
		blocks[2].east = blocks[2].north;
	}
		// Continue here for more textures
	return (rvalue);
}

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

int	cub_parse_textures(t_game *game, const char **str_ptr, t_memory *memory)
{
	int			rvalue;
	size_t		match_target;
	const char	*str = *str_ptr;

	stt_texture_init(game->blocks, memory);
	match_target = 6;	// If DOOR is found, then match_target needs to be increased
	while (*str != 0 && match_target > 0)
	{
		rvalue = stt_match_texture(game->mlx, str, game->blocks, &str);
		if (rvalue < 0)
			return (cub_cleanup(game, "Failed to parse texture"));
		else if (rvalue == 0)
			match_target--;
		else
			str++;
	}
	if (match_target != 0)
		return (cub_cleanup(game, "Fewer textures than expected"));
	while (*str != 0 && *str != '1')	// Skips until the map portion
	{
		if (*str == '\n')
			*str_ptr = str + 1;
		str++;
	}
	if (*str == 0)
		return (cub_cleanup(game, "Map in incorrect format"));
	return (0);
}
