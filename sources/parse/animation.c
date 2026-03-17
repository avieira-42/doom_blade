#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "cub_utils.h"
#include "cmlx.h"

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
int	stt_load_sheet(t_xvar *mlx, t_mat32 *sprites, t_str path, size_t count)
{
	size_t			i;
	t_img			*img;
	const size_t	stride = sprites->rows * sprites->cols;

	i = 2;
	sprites->ptr += stride;
	while (i < count)
	{
		img = stt_load_img(mlx, path, i);
		if (img == NULL)
			return (-1);
		ft_memcpy(sprites->ptr, img->data, stride * sizeof(uint32_t));
		mlx_destroy_image(mlx, img);
		sprites->ptr += stride;
		i++;
	}
	return (0);
}

// Saves images sequentially in memory, in a row x col x depth matrix
int	cub_read_spritesheet(t_xvar *mlx, t_mat32 *sprites, char base_path[256], size_t count)
{
	t_anim			anim;
	t_img			*img;
	const t_str		path = {base_path, ft_strlen(base_path)};

	ft_memset(&anim, 0, sizeof(anim));
	img = stt_load_img(mlx, path, 1);
	sprites->rows = img->height;
	sprites->cols = img->width;
	sprites->depth = count;
	sprites->ptr = malloc(sprites->cols * sprites->rows * sprites->depth * sizeof(uint32_t));
	if (sprites->ptr == NULL)
		return (mlx_destroy_image(mlx, img), -1);
	ft_memcpy(sprites->ptr, img->data, sprites->cols * sprites->rows * sizeof(uint32_t));
	mlx_destroy_image(mlx, img);
	if (stt_load_sheet(mlx, sprites, path, count) == -1)
		return (free(sprites->ptr), -1);
	return (0);
}
