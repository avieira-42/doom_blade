#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <math.h>
#include "cub_structs.h"
#include "cub_utils.h"

// static
// void	stt_blend_argb(uint32_t *restrict dst, const uint32_t *restrict src, size_t length)
// {
// 	size_t	i;

// 	i = 0;
// 	while (i < length)
// 	{
// 		if (src[i] != 0xFF000000)
// 			dst[i] = src[i];
// 		i++;
// 	}
// }

// int	cub_draw_image(t_mat32 src, t_mat32 dst, size_t x_corner, size_t y_corner)
// {
// 	const uint32_t	*src_last_line = src.ptr + src.width * src.height;

// 	if ((src.width + x_corner > dst.width) || (src.height + y_corner > dst.height))
// 		return (-1);	// Image does not fit
// 	dst.ptr += (y_corner * dst.width + x_corner);
// 	while (src.ptr < src_last_line)
// 	{
// 		// ft_memcpy(dst.ptr, src.ptr, src.cols * sizeof(uint32_t));
// 		stt_blend_argb(dst.ptr, src.ptr, src.width);
// 		dst.ptr += dst.width;
// 		src.ptr += src.width;
// 	}
// 	return (0);
// }

void	frame_pixel_put(t_mat32 frame, int32_t x, int32_t y, uint32_t color)
{
	uint32_t	*dst;
	if (x < 0 || x >= frame.width || y < 0 || y >= frame.height)
		return ;
	dst = frame.ptr + x + frame.width * y;
	*(unsigned int *)dst = color;
}

void	cub_draw_texture(t_mat32 frame, t_mat32 image, t_vec2 pos)
{
	size_t		x;
	size_t		y;
	int32_t		color;

	y = 0;
	while (y < image.height)
	{
		x = 0;
		while (x < image.width)
		{
			color = image.ptr[x + image.width * y];	// REVIEW: here you index manually but below use a helper to index
			if (color != 2228223 && color != 1441791)	// REVIEW: colors are hex coded normally
				frame_pixel_put(frame, pos.x.i + x, pos.y.i + y, color);
			x++;
		}
		y++;
	}
}
