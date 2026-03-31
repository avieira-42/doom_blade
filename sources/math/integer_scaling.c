#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "cub_utils.h"
#include "cub_structs.h"

static inline
void	stt_linecpy_t(uint32_t *restrict dst, t_mat32 src, size_t factor)
{
	size_t			i;
	const uint32_t	*src_end = src.ptr + src.width * src.height;
	uint32_t		tmp;

	while (src.ptr < src_end)
	{
		i = 0;
		tmp = *src.ptr;
		while (i < factor)
		{
			*dst++ = tmp;
			i++;
		}
		src.ptr += src.stride;
	}
}

// TODO: Add pos start for draw on DST, assumption isnt scale to fit
void	ft_integer_scaling_t(t_mat32 src, t_mat32 dst, size_t factor)
{
	size_t			i;
	uint32_t		*src_end;
	uint32_t		*odst;
	const size_t	line_size = src.width * factor * sizeof(uint32_t);

	src_end = src.ptr + src.height;
	while (src.ptr < src_end)
	{
		odst = dst.ptr;
		stt_linecpy_t(dst.ptr, src, factor);	// Copies the line, 123 becomes 112233
		dst.ptr += dst.stride;
		i = 1;
		while (i < factor)
		{
			ft_memcpy(dst.ptr, odst, line_size);	// Copies the line across N rows
			dst.ptr += dst.stride;
			i++;
		}
		src.ptr += 1;
	}
}
