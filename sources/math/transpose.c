#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "cub_utils.h"

void	sqr_transpose(uint32_t *src, uint32_t length)
{
	uint32_t	i;
	uint32_t	j;
	uint32_t	tmp;

	i = 0;
	while (i < length)
	{
		j = i + 1;
		while (j < length)
		{
			tmp = src[i * length + j];
			src[i * length + j] = src[j * length + i];
			src[j * length + i] = tmp;
			j++;
		}
		i++;
	}
}

static inline
void	stt_transpose(uint32_t *src, uint32_t *dst, uint32_t cols, uint32_t rows)
{
	uint32_t		i;
	uint32_t		j;

	i = 0;
	while (i < rows)
	{
		j = 0;
		while (j < cols)
		{
			dst[j * rows + i] = src[i * cols + j];
			j++;
		}
		i++;
	}
}

// Possible to remove malloc requirement with block transpose
// Passing a NULL ptr to dst mallocs the required buffer memory
int	ft_transpose(t_mat32 *src)
{
	const size_t	mat_size = (size_t)src->width * (size_t)src->height * sizeof(uint32_t);
	uint32_t		*dst;

	if (src->width == src->height)
	{
		sqr_transpose(src->ptr, src->width);
		return (0);
	}
	dst = malloc(mat_size);
	if (dst == NULL)
		return (-1);
	stt_transpose(src->ptr, dst, src->width, src->height);
	ft_memcpy(src->ptr, dst, mat_size);
	free(dst);
	src->stride = src->height;	// Texture is transposed
	return (0);
}

int	ft_transpose_img(uint32_t *ptr, size_t width, size_t height)
{
	const size_t	mat_size = (size_t)width * (size_t)height * sizeof(uint32_t);
	uint32_t		*dst;

	if (width == height)
	{
		sqr_transpose(ptr, width);
		return (0);
	}
	dst = malloc(mat_size);
	if (dst == NULL)
		return (-1);
	stt_transpose(ptr, dst, width, height);
	ft_memcpy(ptr, dst, mat_size);
	free(dst);
	return (0);
}
