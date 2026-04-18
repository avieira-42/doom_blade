#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cmlx_base.h"
#include "cub_utils.h"
#include "cub_defines.h"

// Assumes source is not transposed
static inline
uint32_t	stt_nearest_neighbour(const t_mat32 *src, t_vec2 norm_pos)
{
	uint32_t	x;
	uint32_t	y;

	x = roundf(norm_pos.x.f * (src->width - 1));
	y = roundf(norm_pos.y.f * (src->height - 1));
	return (src->ptr[y * src->stride + x]);
}

// Scales a texture, storing the transposed result in dst
void	ft_near_scale(t_mat32 dst, t_mat32 src)
{
	uint32_t		*ptr;
	uint32_t		x;
	uint32_t		y;
	t_vec2			norm_pos;
	const t_vec2	delta = {.x.f = 1.0 / (dst.width - 1),
		.y.f = 1.0 / (dst.height - 1)};

	x = 0;
	norm_pos.x.f = 0.0f;
	while (x < dst.width)
	{
		y = 0;
		norm_pos.y.f = 0.0f;
		ptr = dst.ptr + x * dst.stride;
		while (y < dst.height)
		{
			ptr[y] = stt_nearest_neighbour(&src, norm_pos);
			norm_pos.y.f += delta.y.f;
			y++;
		}
		norm_pos.x.f += delta.x.f;
		x++;
	}
}

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
SDL_Surface	*stt_read_xpm(const char *filename, const char **filename_ptr) // edited
{
	char		buffer[256];
	char		*path;
	const char	*path_end = buffer + sizeof(buffer) - 1;

	path = buffer;
	while (isspace(*filename))
		filename++;
	while (*filename != 0 && !isspace(*filename))
	{
		if (path >= path_end)
			return (NULL);	// Path too long, TODO: print
		*path++ = *filename++;
	}
	*path = 0;
	if (filename_ptr != NULL)
		*filename_ptr = filename;
	return (IMG_Load(buffer));
}

// Todo: guarantee lower than render res
int	cub_read_xpm(t_game *game, t_mat32 *dst, const char *filename, const char **filename_ptr)
{
	SDL_Surface	*im;
	SDL_Surface	*im32;
	t_mat32		src;
	size_t		sz;

	im = stt_read_xpm(filename, filename_ptr); // edited
	if (im == NULL)
		return (cub_cleanup(game, "Failed to load image"));
	im32 = SDL_ConvertSurfaceFormat(im, SDL_PIXELFORMAT_ARGB8888, 0);
	SDL_FreeSurface(im);
	if (im32 == NULL)
		return (cub_cleanup(game, "Failed to load image"));
	if (dst->ptr == NULL)	// DST needs to have depth defined
	{
		sz = (size_t)(im32->h * im32->w) * sizeof(uint32_t) * dst->depth;
		*dst = (t_mat32){malloc(sz), im32->w, im32->h, dst->depth, im32->h};
		if (dst->ptr == NULL)
		{
			SDL_FreeSurface(im32);
			return (cub_cleanup(game, "Error_leading dst data buffer\n"));
		}
	}
	src = (t_mat32){(uint32_t *)im32->pixels, im32->w, im32->h, 1, im32->w};
	ft_near_scale(*dst, src);
	SDL_FreeSurface(im32);
	stt_clean_texture(dst->ptr, dst->height * dst->width);
	if (src.width != dst->width || src.height != dst->height)
		return (cub_cleanup(game, "Sheet frame dimensions do not match"));
	return (0);
}
