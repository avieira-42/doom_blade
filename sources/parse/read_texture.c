#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game_types.h"
#include "game_prototypes.h"
#include "game_defines.h"

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
SDL_Surface	*stt_read_texture(const char *filename, const char **filename_ptr) // edited
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
int	cub_read_texture(t_game *game, t_mat32 *dst, const char *filename, const char **filename_ptr)
{
	SDL_Surface	*im;
	SDL_Surface	*im32;
	t_mat32		src;
	size_t		sz;

	im = stt_read_texture(filename, filename_ptr); // edited
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
	ft_near_scale_t(*dst, src);
	SDL_FreeSurface(im32);
	stt_clean_texture(dst->ptr, dst->height * dst->width);
	if (src.width != dst->width || src.height != dst->height)
		return (cub_cleanup(game, "Sheet frame dimensions do not match"));
	return (0);
}
