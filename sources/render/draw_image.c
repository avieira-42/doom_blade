#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_structs.h"
#include "cub_utils.h"

// Draws an image on top of another image, aligned to the left corner
// Can be used to render

typedef struct s_transform
{
	t_vec2	pos;
	t_vec2	scale;
}	t_form;

int	cub_draw_image(t_mat32 src, t_mat32 dst, size_t x_corner, size_t y_corner)
{
	const size_t	src_line_size = src.cols * sizeof(uint32_t);
	const uint32_t	*src_last_line = src.ptr + src.cols * src.rows;

	if ((src.cols + x_corner > dst.cols) || (src.rows + y_corner > dst.rows))
		return (-1);	// Image does not fit
	dst.ptr += (y_corner * dst.cols + x_corner);
	while (src.ptr <= src_last_line)
	{
		ft_memcpy(dst.ptr, src.ptr, src_line_size);
		dst.ptr += dst.cols;
		src.ptr += src.cols;
	}
	return (0);
}
