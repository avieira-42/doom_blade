#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

static
void	stt_blend_argb(uint32_t *restrict dst, const uint32_t *restrict src, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		if (src[i] != 0xFF000000)
			dst[i] = src[i];
		i++;
	}
}

int	cub_draw_image(t_mat32 src, t_mat32 dst, size_t x_corner, size_t y_corner)
{
	const uint32_t	*src_last_line = src.ptr + src.cols * src.rows;

	if ((src.cols + x_corner > dst.cols) || (src.rows + y_corner > dst.rows))
		return (-1);	// Image does not fit
	dst.ptr += (y_corner * dst.cols + x_corner);
	while (src.ptr < src_last_line)
	{
		// ft_memcpy(dst.ptr, src.ptr, src.cols * sizeof(uint32_t));
		stt_blend_argb(dst.ptr, src.ptr, src.cols);
		dst.ptr += dst.cols;
		src.ptr += src.cols;
	}
	return (0);
}
