#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "cub_defines.h"
#include "cub_structs.h"
#include "cub_utils.h"
#include "cmlx.h"
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

// Reads an xpm texture, scales it to tex_height, transposes the result
// and frees the original MLX img ptr
int	cub_read_texture(t_xvar *mlx, t_mat32 *texture, const char *filename, const char **filename_ptr)
{
	t_vec2	scale;
	t_img	*img;

	if (texture->ptr != NULL)
		return (-2);
	img = stt_read_xpm(mlx, filename, filename_ptr);
	if (img == NULL)
		return (-1);
	texture->height = TEX_HEIGHT;
	texture->width = (double)texture->height / img->height * img->width;
	texture->depth = 1;
	texture->stride = texture->width;
	texture->ptr = malloc(texture->width * texture->height * sizeof(uint32_t));
	if (texture->ptr == NULL)
		return (mlx_destroy_image(mlx, img), -1);
	scale.x.f = (double)texture->width / img->width;
	scale.y.f = (double)texture->height / img->height;
	ft_bilinear_scaling(&(t_mat32){(uint32_t*)img->data, img->width, img->height, 1, img->width}, texture, scale, (t_vec2){0});
	ft_transpose(texture);
	mlx_destroy_image(mlx, img);
	return (0);
}

static
int	stt_read_color(t_xvar *mlx, t_mat32 *texture, const char *filename, const char **filename_ptr)
{
	size_t		i;
	uint32_t	color;

	if (texture->ptr != NULL)
		return (-1);	// TODO: Print error
	while (ft_isspace(*filename))
		filename++;
	i = 0;
	while (filename[i] >= '0' && filename[i] <= '9')
		i++;
	if (filename[i] != ',')
		return (cub_read_texture(mlx, texture, filename, filename_ptr));
	color = ft_strtoargb(filename, filename_ptr);
	texture->ptr = malloc(TEX_HEIGHT * sizeof(uint32_t));		// Creates a column of colors
	if (texture->ptr == NULL)
		return (-1);	// TODO: Print error
	texture->height = TEX_HEIGHT;	// Consider it transposed
	texture->width = 1;
	texture->stride = TEX_HEIGHT;
	i = 0;
	while (i < TEX_HEIGHT)
		texture->ptr[i++] = color;
	return (0);
}

static inline
int	stt_match_texture(t_xvar *mlx, const char *str, t_block *blocks, const char **str_ptr)
{
	int	rvalue;

	while (ft_isspace(*str))	// TODO: Check that this doesnt break anything
		str++;
	rvalue = 1;
	if (str[0] == 'N' && str[1] == 'O')
		rvalue = cub_read_texture(mlx, &blocks[1].north, str + 2, str_ptr);
	else if (str[0] == 'E' && str[1] == 'A')
		rvalue = cub_read_texture(mlx, &blocks[1].east, str + 2, str_ptr);
	else if (str[0] == 'S' && str[1] == 'O')
		rvalue = cub_read_texture(mlx, &blocks[1].south, str + 2, str_ptr);
	else if (str[0] == 'W' && str[1] == 'E')
		rvalue = cub_read_texture(mlx, &blocks[1].west, str + 2,  str_ptr);
	else if (str[0] == 'F')
		rvalue = stt_read_color(mlx, &blocks[0].south, str + 1, str_ptr);
	else if (str[0] == 'C')
		rvalue = stt_read_color(mlx, &blocks[0].north, str + 1, str_ptr);
		// Continue here for more textures
	return (rvalue);
}

int	cub_read_textures(t_xvar *mlx, const char *str, const char **str_ptr, t_block *blocks)
{
	int		rvalue;
	size_t	match_target;

	match_target = 6;	// If DOOR is found, then match_target needs to be increased
	while (*str != 0 && match_target > 0)
	{
		rvalue = stt_match_texture(mlx, str, blocks, &str);
		if (rvalue < 0)
			return (-1);	// Future exit cleanup
		else if (rvalue == 0)
			match_target--;
		else
			str++;
	}
	if (match_target != 0)
		return (-1);
	while (*str != 0 && *str != '1')	// Skips until the map portion
	{
		if (*str == '\n')
			*str_ptr = str + 1;
		str++;
	}
	return (-(*str == 0));
}
