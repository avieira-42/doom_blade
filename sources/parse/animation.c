#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_utils.h"
#include "cmlx_base.h"

#define SENTINEL_VALUE 0xFF000000 // Color value that should be transparent

static
t_img	*stt_load_img(t_xvar *mlx, t_str path, size_t count)
{
	t_img				*img;
	static const char	append_str[5] = ".xpm";
	int					tmp[2];
	size_t				number_length;
	char				*append_ptr;

	append_ptr = path.ptr + path.length;
	number_length = ft_itoa_r((int64_t)count, append_ptr);
	append_ptr += number_length;
	ft_memcpy(append_ptr, append_str, sizeof(append_str));
	img = mlx_xpm_file_to_image(mlx, path.ptr, tmp, tmp + 1);
	return (img);
}

static
int	stt_load_sheet(t_xvar *mlx, t_mat32 sprite, t_str path, size_t count)
{
	size_t			i;
	t_img			*img;
	const size_t	depth_stride = sprite.height * sprite.width;

	i = 1;
	sprite.ptr += depth_stride;
	while (i < count)
	{
		img = stt_load_img(mlx, path, i);
		if (img == NULL)
			return (-1);
		if (img->width != sprite.width || img->height != sprite.height)	// Image sizes are different
			return (mlx_destroy_image(mlx, img), -1);
		ft_transpose_img((uint32_t *)img->data, (size_t)img->width, (size_t)img->height);
		ft_memcpy(sprite.ptr, img->data, depth_stride * sizeof(uint32_t));
		mlx_destroy_image(mlx, img);
		sprite.ptr += depth_stride;
		i++;
	}
	return (0);
}

void	stt_clean_texture(t_mat32 texture)
{
	size_t			i;
	const size_t	length = texture.height * texture.width * texture.depth;

	i = 0;
	while (i < length)
	{
		if (texture.ptr[i] == SENTINEL_VALUE)
			texture.ptr[i] = 0;
		i++;
	}
}

// Saves images sequentially in memory, in a row x col x depth matrix
// TODO: Change this to receive a sheet pointer and return error
t_sheet cub_read_spritesheet(t_game *game, const char *base_path, size_t count, long frame_time)
{
	t_sheet		sheet;
	t_mat32		texture;
	t_img		*img;
	char		buffer[256];
	const t_str	path = {buffer, ft_strlen(base_path)};

	ft_memset(&sheet, 0, sizeof(sheet));
	if (path.length + 32 > sizeof(buffer))
		return (cub_cleanup(game, "Texture path is too long"), sheet);
	ft_memcpy(buffer, base_path, path.length + 1);
	img = stt_load_img(game->mlx, path, 0);
	if (img == NULL)
		return (cub_cleanup(game, "Failed to load MLX image"), sheet);
	texture = (t_mat32){0, img->width, img->height, count, img->width};
	texture.ptr = malloc((size_t)(img->height * img->width) * count * sizeof(uint32_t));
	if (texture.ptr == NULL)
		return (mlx_destroy_image(game->mlx, img), cub_cleanup(game, "Malloc failure"), sheet);
	ft_memcpy(texture.ptr, img->data, texture.width * texture.height * sizeof(uint32_t));
	ft_transpose(&texture);
	mlx_destroy_image(game->mlx, img);
	if (stt_load_sheet(game->mlx, texture, path, count) == -1)
		return (free(texture.ptr), cub_cleanup(game, "Failed to load sheet"), sheet);
	sheet.texture = texture;
	sheet.count = count;
	sheet.frame_size = texture.width * texture.height;
	sheet.frame_time = frame_time;
	sheet.index = 0;
	return (sheet);
}
