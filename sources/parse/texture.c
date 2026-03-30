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

static
t_mat32	stt_read_color(const char *argb_str, const char **argb_strptr)
{
	size_t		i;
	uint32_t	color;
	t_mat32		texture;

	color = ft_strtoargb(argb_str, argb_strptr);
	texture.ptr = malloc(TEX_HEIGHT * sizeof(uint32_t));		// Creates a column of colors
	if (texture.ptr == NULL)
		return (texture);	// TODO: Print error
	texture.width = TEX_HEIGHT;
	texture.height = 1;
	texture.stride = TEX_HEIGHT;
	i = 0;
	while (i < TEX_HEIGHT)
		texture.ptr[i++] = color;
	return (texture);
}

t_mat32	cub_read_texture2(t_xvar *mlx, const char **filename_ptr, bool transpose)
{
	size_t		i;
	const char	*filename = *filename_ptr;

	while (ft_isspace(*filename))
		filename++;
	i = 0;
	while (filename[i] >= '0' && filename[i] <= '9')
		i++;
	if (filename[i] != ',')
		return (stt_read_texture(mlx, texture, filename, filename_ptr));
	else
		return (stt_read_color());
}
