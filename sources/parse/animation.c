#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_defines.h"
#include "cub_utils.h"
#include "cmlx_base.h"

static inline
void	stt_clean_texture(uint32_t *ptr, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (ptr[i] == ALPHA_SENTINEL1 || ptr[i] == ALPHA_SENTINEL2)
			ptr[i] = 0;
		i++;
	}
}

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
	uint32_t		*optr;
	const size_t	depth_stride = sprite.height * sprite.width;

	i = 1;
	optr = sprite.ptr;
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
	stt_clean_texture(optr, depth_stride * sprite.depth);
	return (0);
}

// Saves images sequentially in memory, in a row x col x depth matrix
// TODO: Change this to receive a sheet pointer and return error
t_sheet cub_readsheet(t_game *game, const char *base_path, size_t count, long frame_time)
{
	t_mat32		tex;
	t_img		*img;
	char		buffer[256];
	const t_str	path = {buffer, ft_strlen(base_path)};

	if (path.length + 32 > sizeof(buffer))
		return (cub_cleanup(game, "Texture path is too long"), (t_sheet){0});
	ft_memcpy(buffer, base_path, path.length + 1);
	img = stt_load_img(game->mlx, path, 0);
	if (img == NULL)
		return (cub_cleanup(game, "Failed to load MLX image"), (t_sheet){0});
	tex = (t_mat32){0, img->width, img->height, count, img->width};
	tex.ptr = malloc((size_t)(img->height * img->width) * count * sizeof(uint32_t));
	if (tex.ptr == NULL)
		return (mlx_destroy_image(game->mlx, img), cub_cleanup(game, "Malloc failure"), (t_sheet){0});
	ft_memcpy(tex.ptr, img->data, tex.width * tex.height * sizeof(uint32_t));
	ft_transpose(&tex);
	mlx_destroy_image(game->mlx, img);
	if (stt_load_sheet(game->mlx, tex, path, count) == -1)
		return (free(tex.ptr), cub_cleanup(game, "Failed to load sheet"), (t_sheet){0});
	return ((t_sheet){.texture = tex, .count = count, .index = 0,
		.frame_time = frame_time, .frame_size = tex.width * tex.height});
}
